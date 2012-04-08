#include "map.h"

ERROR(NullMapSize, "%s of map must be not null.") // "Width"
ERROR(InvalidFileFormat, "Map file is incorrect: %s.") // "smth is invalid"

bool is_separator(char ch)
{
    return ch == '\n' || ch == '\r' || ch == '\t';
}

Map::Map(FILE *file, unsigned screen_position_x, unsigned screen_position_y, Game *game) : game(game)
{
    tick_counter = 0;

    energizers_count = 0;
    while (true)
    {
        int ch = fgetc(file);
        if (ch == MAP_ENERGIZER)
            ++energizers_count;
        else if (ch == EOF)
            break;
    }
    energizers = new Cell*[energizers_count];


    fseek(file, 0, SEEK_SET);

    if (fscanf(file, "%u %u", &width, &height) < 2)
        throw InvalidFileFormatError("no size definition in first line");

    if (fgetc(file) != ';')
        throw InvalidFileFormatError("';' expected after size definition");

    if (width == 0)
        throw NullMapSizeError("Width");
    else if (height == 0)
        throw NullMapSizeError("Height");

    init_positions.pacman = NULL;
    for( unsigned i = 0 ; i < GHOST_NUMBER ; ++i )
        init_positions.ghosts[i] = NULL;

    unsigned size = width*height;
    cells = (Cell *)malloc(size * sizeof(cells[0]));
    unsigned i = 0;
    unsigned ghost_index = 0;
    unsigned energizer_index = 0;
    points_num = 0;
    while (true)
    {
        int ch = fgetc(file);
        if (ch == EOF)
            throw InvalidFileFormatError("detected EOF but not all map cells defined");
        else if (is_separator(ch))
            continue;
        else
        {
            if (i >= size)
                throw InvalidFileFormatError("too many map cells defined");

            ObjType ot;

            switch ( ch )
            {
                case MAP_PACMAN:
                    if (init_positions.pacman != NULL)
                        throw InvalidFileFormatError("too many PacMans defined");

                    init_positions.pacman = &cells[i];
                    ot = EMPTY;
                    break;

                case MAP_GHOST:
                    if( ghost_index >= GHOST_NUMBER )
                        throw InvalidFileFormatError("too many ghosts defined (need " TO_STR(GHOST_NUMBER) ")");
                    init_positions.ghosts[ghost_index] = &cells[i];
                    ot = EMPTY;
                    ++ghost_index;
                    break;

                case MAP_POINT:
                    ++points_num;
                    ot = POINT;
                    break;

                case MAP_EMPTY:
                    ot = EMPTY;
                    break;
                case MAP_ENERGIZER:
                    ot = ENERGIZER;
                    energizers[energizer_index++] = &cells[i];
                    break;
                case MAP_WALL:
                    ot = WALL;
                    break;

                default :
                    char buffer[] = "invalid symbol in map cell definition (X)";
                    buffer[ strlen(buffer) - 2 ] = ch;
                    throw InvalidFileFormatError(buffer);
            }

            new(&cells[i]) Cell (ot,
                                    &cells[ (i - width + size) % size ],
                                    &cells[ ( (i+1) % width == 0 ) ? (i + 1 - width ) : (i + 1) ],
                                    &cells[ (i + width) % size ],
                                    &cells[ (   i   % width == 0 ) ? (i - 1 + width ) : (i - 1) ],
                                    screen_position_x + 2*(i % width),
                                    screen_position_y + (i / width),
                                    this
                                );
            ++i;
            if (i == size)
                break;
        }
    }
    if ( init_positions.pacman == NULL )
        throw InvalidFileFormatError("one PacMan must be defined");

    if ( ghost_index < GHOST_NUMBER )
        throw InvalidFileFormatError("too few ghosts defined (need " TO_STR(GHOST_NUMBER) ")");

    fclose(file);
}

unsigned Map::get_ticks()
{
    return tick_counter;
}

Map::~Map()
{
    unsigned size = width * height;
    for(unsigned i = 0; i < size; ++i)
        cells[i].~Cell();
    free(cells);
    delete[] energizers;
}

void Map::set_cells_loop()
{
    for (unsigned i = 0; i < height; ++i)
        for (unsigned j = 0; j < width; ++j)
            cells[ i*width + j ].setLoop( getLoop() );
}

void Map::draw()
{
    for (unsigned i = 0; i < height; ++i)
        for (unsigned j = 0; j < width; ++j)
            getLoop()->postMessage(&cells[ i*width + j ], MSG_DRAW, Data(0));
}

void Map::reset_wave()
{
    for (unsigned i = 0; i < height; ++i)
        for (unsigned j = 0; j < width; ++j)
            cells[i*width + j].wave.reset();
}

ObjType Map::who_is_here(Cell *cell)
{
    if (cell == game->get_pacman()->get_position() && game->get_pacman()->is_alive())
        return PACMAN;
    for( unsigned i = 0 ; i < GHOST_NUMBER ; ++i )
    {
        if (cell == game->get_ghost(i)->get_position() && game->get_ghost(i)->is_alive())
            return GHOST;
    }
    return EMPTY;
}

void Map::pop_point()
{
    --points_num;
    assert(points_num >= 0);
    if( points_num == 0 )
        getLoop()->postMessage(NULL, MSG_STOP_GAME, Data(STOP_POINTS_EATEN));
}

Cell * Map::init_pos_pacman()
{
    return init_positions.pacman;
}

Cell * Map::init_pos_ghost(unsigned index)
{
    assert( index < 4 );
    return init_positions.ghosts[index];
}

void Map::dump_wave(FILE *file)
{
    for (unsigned i = 0; i < height; ++i)
    {
        for (unsigned j = 0; j < width; ++j)
        {
            Cell *cell = &cells[ i*width + j ];
            fprintf(file, "%3i ", cell->wave.depth);
        }
        fputc('\n', file);
    }
}

bool Map::processMessage(int message, Data extra_data)
{
    if (message == MSG_TICK)
    {
        for( unsigned i = 0 ; i < energizers_count ; ++i )
            if (energizers[i]->get_content() == ENERGIZER)
                getLoop()->postMessage(energizers[i], MSG_DRAW, Data());

        ++tick_counter;
    }
    return false;
}


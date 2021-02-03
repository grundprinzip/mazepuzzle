import sys
import random

WIDTH=int(15)
HEIGHT=int(15)
WALL=1
HALLWAY=1
WALL = "#"
FLOOR = " "



def pm(data):
    for x in range(WIDTH+2):
        sys.stdout.write("-")
    sys.stdout.write("\n|")
    for x in range(WIDTH*HEIGHT):
        if x > 0 and x % WIDTH == 0:
            sys.stdout.write("|\n|")
        sys.stdout.write(data[x])
    sys.stdout.write("|\n")
    for x in range(WIDTH+2):
        sys.stdout.write("-")
    sys.stdout.write("\n")



def nbs(data, cell):
    result = set()
    # North
    nb_n = cell - WIDTH
    if valid_col(nb_n) and data[nb_n] == WALL:
        result.add(nb_n)
    # South
    nb_s = cell + WIDTH
    if valid_col(nb_s) and data[nb_s] == WALL:
        result.add(nb_s)
    # East
    nb_e = cell + 1
    if same_row(nb_e, cell) and data[nb_e] == WALL:
        result.add(nb_e)
    # West
    nb_w = cell - 1
    if same_row(nb_w, cell) and data[nb_w] == WALL:
        result.add(nb_w)

    return result

def valid_col(c):
    return c >= 0 and c < (WIDTH*HEIGHT)

def same_row(c1, c2):
    return int(c1 / WIDTH) == int(c2 / WIDTH)

def is_floor(data, c):
    return data[c] == FLOOR

def generate(data):
    """ Prims algorithm """

    # Create starting cell
    data[0] = FLOOR
    walls = set()

    # Add the two first walls
    walls.add(1)
    walls.add(WIDTH)

    # Pick one of the walls
    while len(walls) > 0:
        print("Step")
        cell = random.choice(list(walls))
        walls.remove(cell)

        print(cell, data[cell])
        # Check on which side we have the passage

        # North
        nb_n = cell - WIDTH
        # South
        nb_s = cell + WIDTH
        # East
        nb_e = cell + 1
        # West
        nb_w = cell - 1

        #print(nb_n, nb_e, nb_s, nb_w)
        #print(is_floor(data, nb_n) ^ is_floor(data, nb_s))
        #print(is_floor(data, nb_e) ^ is_floor(data, nb_w))
        #print(same_row(nb_e, cell), same_row(nb_w, cell))

        # Check if both valid and check if only one of them is passage
        if valid_col(nb_n) and valid_col(nb_s) and is_floor(data, nb_n) ^ is_floor(data, nb_s):
            # Mark the passage
            data[cell] = FLOOR

            # Add the neighbors to the list
            if is_floor(data, nb_n):
                data[nb_s] = FLOOR
                nn = nbs(data, nb_s)
            else:
                data[nb_n] = FLOOR
                nn= nbs(data, nb_n)

            walls = walls.union(nn)
        elif same_row(nb_e, cell) and same_row(nb_w, cell) and is_floor(data, nb_e) ^ is_floor(data, nb_w):
            data[cell] = FLOOR

            if is_floor(data, nb_e):
                data[nb_w] = FLOOR
                nn= nbs(data, nb_w)
            else:
                data[nb_e] = FLOOR
                nn = nbs(data, nb_e)
            walls = walls.union(nn)

    return data

if __name__ == "__main__":
    # The maze
    data = [WALL] * (WIDTH * HEIGHT)
    pm(generate(data))

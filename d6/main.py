import numpy as np
from numba import jit, prange
import matplotlib.pyplot as plt


def read_file(filename):

    state = np.genfromtxt(filename, dtype=np.int8, delimiter=",")
    print(f"loaded state shape: {state.shape}")
    return state


@jit(nopython=True, parallel=True)
def update_states(state):
    new_spawn = 0
    for i in prange(state.shape[0]):
        state[i] -= 1
        if state[i] == -1:
            new_spawn += 1
            state[i] = 6
    return state, new_spawn


@jit(nopython=True)
def extend_state(state, new_spawn):
    return np.append(state, np.ones(new_spawn,)*8)


def evolve(state, days):
    new_spawn = 0
    fishes = state.shape[0]
    itr = 0
    while itr < days:
        if state.shape[0] / fishes >= 2:
            fishes = state.shape[0]
            print(f"{itr} : {fishes}")

        state, new_spawn = update_states(state)
        state = extend_state(state, new_spawn)
        itr += 1
    return state.shape[0]


if __name__ == "__main__":
    days = 200
    filename = "input"
    # filename = "test1"

    state = read_file(filename)
    print(state.shape)
    print(state.shape[0]*2 * (days/(18-12)))

    seg_fish = evolve(state, days)
    print(f"Fish after {days} days: {seg_fish}")

import numpy as np
from numba import jit, prange


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
    while days > 0:
        print(f"Days remaining {days:.0f}")
        state, new_spawn = update_states(state)
        state = extend_state(state, new_spawn)
        days -= 1

    return state


if __name__ == "__main__":
    days = 256
    filename = "input"

    state = read_file(filename)
    print(state.shape)
    state = evolve(state, days)
    print(f"Fish after {days} days: {state.shape[0]}")

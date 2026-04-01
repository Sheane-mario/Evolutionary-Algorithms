## Variations of Hill Climbing in This Project

This project implements several variants of hill climbing over a continuous search space using a simple **sphere function** as the objective:

$$f(\mathbf{x}) = \sum_{i=1}^{D} x_i^2,$$

where $D$ is the dimensionality of the search space. Because the code treats lower cost as worse and uses **fitness = -cost**, the algorithms effectively **maximize**

$$\text{fitness}(\mathbf{x}) = -f(\mathbf{x}) = - \sum_{i=1}^{D} x_i^2,$$

whose global optimum is at $\mathbf{x} = \mathbf{0}$ with fitness $0$.

All variants share the same basic representation and utility functions.

---

## Common Setup

- **Dimension**: `DIMENTIONS = 6` (size of the feature vector).
- **Variable bounds**: `LOWER_BOUND = -6.42`, `UPPER_BOUND = 6.42`.
- **Step size**: `STEP_SIZE = 0.01` (used to bound mutation size).
- **Representation**: Each solution is an `Individual`:
	- `features`: `std::vector<double>` of size `DIMENTIONS`.
	- `fitness`: `double`, computed by `fitness_measure`.
- **Initialization**:
	- Each feature is sampled uniformly from `[LOWER_BOUND, UPPER_BOUND]` using `dist(gen)`.
- **Fitness function** (`fitness_measure`):
	- Computes the negative squared norm of the feature vector.
	- Higher fitness means closer to the origin.

Randomness is handled via global `std::mt19937 gen` and several distributions:

- `dist`: uniform over the search bounds for initialization.
- `random_mutation`: uniform over `[-STEP_SIZE, STEP_SIZE]` for local perturbations.
- `probability_distribution`: uniform over `[0, 1]` for probabilistic mutation decisions.
- `int_dist`: uniform integer over feature indices for random selection.

Every mutation step is clamped back into `[LOWER_BOUND, UPPER_BOUND]` using `std::max`/`std::min`.

---

## 1. Steepest Ascent Hill Climbing

### Algorithmic Idea

Steepest ascent hill climbing evaluates **all neighbors obtained by mutating each feature individually**, then **moves to the single best neighbor** if it improves fitness. If no neighbor is better, the search stops at a local optimum.

Informally:

1. Start from a random solution `x0`.
2. Generate neighbors by mutating one feature at a time.
3. Evaluate all neighbors.
4. Move to the neighbor with **maximum fitness** if it is better than current.
5. Repeat until no improving neighbor exists.

This makes relatively large, informed moves at each iteration but can be expensive in high dimensions because it evaluates many neighbors per step.

### Implementation Summary (`steepest_ascent`)

- Initialize `x0` with random features and compute its fitness.
- In each iteration:
	- Create a `neighbours` vector and insert the current individual `x0`.
	- For each feature index `q` from `0` to `DIMENTIONS - 1`:
		- Copy `x0` to `xq`.
		- Mutate only the `q`-th feature by adding a sample from `random_mutation`.
		- Clamp `xq.features[q]` into bounds.
		- Compute `xq.fitness`.
		- Push `xq` into `neighbours`.
	- Use `std::max_element` over `neighbours` (by fitness) to find the best neighbor `x_p`.
	- If `x_p->fitness` is **not greater** than `x0.fitness`, stop (local optimum found).
	- Otherwise, set `x0 = *x_p` and continue.
- Print generation and best fitness each iteration.
- Return the final `x0` as the local optimum.

**Stopping condition**: No single-feature mutation yields a strict fitness improvement.

---

## 2. Next Ascent Hill Climbing

### Algorithmic Idea

Next ascent hill climbing also considers neighbors obtained by single-feature mutation but follows a **first-improvement** or **greedy** strategy:

- It scans features in a fixed order.
- As soon as it finds a mutation that improves fitness, it **accepts it immediately** and restarts the scan from the new point.

This can be more efficient than steepest ascent because it may avoid evaluating all possible neighbors at each step.

Informally:

1. Start from random `x0`.
2. For each feature index `q`:
	 - Mutate feature `q` and compute fitness.
	 - If fitness improves, accept the new individual and restart the loop from the first feature.
3. If a full pass over all features finds no improving mutation, stop.

### Implementation Summary (`next_ascent`)

- Initialize `x0` with random features and its fitness.
- Maintain a boolean `improving` flag.
- While `improving`:
	- Set `improving = false`.
	- For each feature index `q`:
		- Copy `x0` to `xq`.
		- Mutate `xq.features[q]` by `random_mutation` and clamp.
		- Compute `xq.fitness`.
		- If `xq.fitness > x0.fitness`:
			- Assign `x0 = xq` (accept improvement).
			- Set `improving = true`.
			- `break` the loop over `q` to restart from the first feature.
	- Print generation and best fitness.
- When no improvement is found in a full pass, terminate and return `x0`.

**Stopping condition**: A full sweep over all features produces no improving mutation.

---

## 3. Random Mutation Hill Climbing

### Algorithmic Idea

Random mutation hill climbing uses a **stochastic neighbor selection**. Instead of systematically exploring all coordinates, it:

1. Chooses a random feature index.
2. Mutates only that coordinate.
3. Accepts the new solution only if it improves fitness.

This is simpler and can sometimes escape small basins due to its random exploration, but the implementation here does **not** use restarts or probabilistic acceptance of worse moves; it is still a greedy hill climber.

### Implementation Summary (`random_mutation_hill_climbing`)

- Initialize `x0` randomly and compute `x0.fitness`.
- For a fixed number of iterations (`MAX_ITERATIONS = 10000`):
	- Sample an index `q` from `int_dist`.
	- Copy `x0` to `x1`.
	- Mutate feature `q` of `x1` by `random_mutation` and clamp.
	- Compute `x1.fitness`.
	- If `x1.fitness > x0.fitness`, accept the move: `x0 = x1`.
	- Print generation and best fitness.
- After the loop, return `x0`.

**Stopping condition**: After a predefined number of iterations (`MAX_ITERATIONS`), regardless of whether further improvements are available.

---

## 4. Adaptive Hill Climbing

### Algorithmic Idea

Adaptive hill climbing extends random mutation ideas by introducing:

- A **mutation probability** per feature.
- An attempt at **adapting the step size** based on success or failure of moves.

At each iteration, it may mutate multiple features at once, controlled by a per-feature mutation probability `pm`. If the mutated individual is better, it is accepted and the algorithm tries to **expand** the search (intended by adjusting `current_step`); otherwise, it **shrinks** the search step to focus locally.

> Note: In the current code, `current_step` is updated but not actually used inside the mutation distribution. It is conceptually there to represent an adaptive step size.

### Implementation Summary (`adaptive_hill_climbing`)

- Initialize `x0` randomly and compute its fitness.
- Parameters:
	- `pm = 0.1`: per-feature mutation probability.
	- `current_step = STEP_SIZE`: conceptual step size.
	- `adapt_factor = 0.99`: factor for adjusting `current_step`.
	- `MAX_ITERATIONS = 10000`.
- For each iteration `i`:
	- Copy `x0` to `x1`.
	- Set `is_mutated = false`.
	- For each feature index `q`:
		- Draw `r` from `[0, 1]`.
		- If `r < pm`:
			- Mutate `x1.features[q]` by `random_mutation` and clamp.
			- Set `is_mutated = true`.
	- If no feature was mutated, skip evaluation and move to the next iteration.
	- Compute `x1.fitness`.
	- If `x1.fitness > x0.fitness` (success):
		- Accept the new solution: `x0 = x1`.
		- Increase the effective step size: `current_step /= adapt_factor`.
	- Else (failure):
		- Decrease the effective step size: `current_step *= adapt_factor`.
	- Print generation and best fitness.
- After all iterations, return `x0`.

**Stopping condition**: A fixed number of iterations (`MAX_ITERATIONS`), with adaptation happening at each step based on success or failure.

---

## 5. Multi-Restart Strategies (Commented Code in `main`)

In `main`, there is additional (currently commented) logic that demonstrates how to turn single-run hill climbers into **multi-restart** algorithms:

- A global best individual (e.g. `steepest_ascent_global_optima`) is maintained.
- The single-run hill climber (e.g. `steepest_ascent` or `next_ascent`) is called repeatedly up to `MAX_RESTARTS` times.
- After each run, the local optimum is compared against the global best.
- A **stagnation counter** tracks how many runs have occurred without improving the global best.
- If stagnation exceeds `STAGNATION_LIMIT`, the procedure stops early under the assumption that additional restarts are unlikely to improve significantly.

This multi-restart pattern helps overcome local optima by reinitializing from different random starting points.

---

## 6. Practical Notes and Possible Extensions

- **Logging**: Each algorithm prints generation number and best fitness, which is useful for visualizing convergence behaviour.
- **Bounds handling**: All mutations are clamped to the predefined interval, preventing invalid solutions.
- **Fitness vs. cost**: The project maximizes `fitness = -cost`. For other problems, you can directly maximize a reward function or minimize a cost by negating it.
- **Adaptive step usage**: To fully realize adaptive hill climbing, `current_step` can be used to drive the mutation distribution (e.g., scale the mutation range by `current_step`).
- **Higher dimensions / other functions**: The same structure can be reused for other benchmark functions (Rastrigin, Rosenbrock, etc.) or different dimensionalities by modifying `DIMENTIONS` and `fitness_measure`.

Together, these implementations provide a compact experimental framework to compare **systematic** (steepest), **first-improvement** (next ascent), **stochastic** (random mutation), and **adaptive** hill climbing on a simple continuous optimization problem.


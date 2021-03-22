def are_close(values1, values2):
	global threshold
	states = list(values1)
	temp = 0
	for state in states:
		temp += abs(values1[state] - values2[state])
	temp /= len(states)
	return temp < threshold
def initialize_values():
	global size
	global walls
	values = {}
	for i in range(size[0]):
		for j in range(size[1]):
			if (i, j) not in walls:
				values[(i, j)] = 0
	return values
def f(state, action):
	global end
	if state == end:
		return state
	else:
		return (state[0] + action[0], state[1] + action[1])
def rho(state):
	global end
	if state == end:
		return 0
	else:
		return -1
def value_update(state, gamma):
	global values
	l = []
	if (state[0] + 1, state[1]) in list(values):
		l.append(rho(state) + gamma * values[f(state, (1, 0))])
	if (state[0] - 1, state[1]) in list(values):
		l.append(rho(state) + gamma * values[f(state, (-1, 0))])
	if (state[0], state[1] + 1) in list(values):
		l.append(rho(state) + gamma * values[f(state, (0, 1))])
	if (state[0], state[1] - 1) in list(values):
		l.append(rho(state) + gamma * values[f(state, (0, -1))])
	return max(l)
def v_iteration(gamma):
	global values
	new_values = {}
	while True:
		for state in list(values):
			new_values[state] = value_update(state, gamma)
		if are_close(values, new_values):
			break
		values = new_values
def learn_values():
	v_iteration(0.1)
	v_iteration(0.2)
	v_iteration(0.3)
	v_iteration(0.4)
	v_iteration(0.5)
	v_iteration(0.6)
	v_iteration(0.7)
	v_iteration(0.8)
	v_iteration(0.9)
	v_iteration(0.99)
	v_iteration(0.999)
	v_iteration(0.9999)
def optimal_next_state(state):
	global values
	d = {}
	next_state = (state[0] + 1, state[1])
	if next_state in list(values):
		d[next_state] = values[next_state]
	next_state = (state[0] - 1, state[1])
	if next_state in list(values):
		d[next_state] = values[next_state]
	next_state = (state[0], state[1] + 1)
	if next_state in list(values):
		d[next_state] = values[next_state]
	next_state = (state[0], state[1] - 1)
	if next_state in list(values):
		d[next_state] = values[next_state]
	return max(d, key = d.get)
def make_path():
	global start
	global end
	l = [start]
	while True:
		l.append(optimal_next_state(l[-1]))
		if l[-1] == end:
			break
	return l
size = 6, 6
start = 2, 2
end = 5, 4
walls = [(1, 4), (2, 4), (3, 1), (3, 2), (3, 3), (3, 4)]
threshold = 0.005
values = initialize_values()
learn_values()
print(make_path())
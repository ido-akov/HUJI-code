from itertools import combinations

class Node:
	def __init__(self, data, pos = None, neg = None):
		self.data = data
		self.positive_child = pos
		self.negative_child = neg

	def __str__(self, level=0):
		ret = "\t" * level + repr(self.data) + "\n"
		if self.positive_child and self.negative_child:
			for child in [self.positive_child, self.negative_child]:
				ret += child.__str__(level + 1)
		return ret

	def __repr__(self):
		return '<tree node representation>'

	def get_data(self):
		"""
		API function
		:return: node data
		"""
		return self.data

	def is_leaf(self):
		""" API function: checks whether node has children. if not node is leaf and returns True. Else False"""

		if not self.positive_child:  #either two children or none
			return True
		return False

	def get_pos_child(self):
		return self.positive_child

	def get_neg_child(self):
		return self.negative_child


class Record:

	def __init__(self, illness, symptoms):
		self.illness = illness
		self.symptoms = symptoms

	def get_symptoms(self):
		""" API function"""
		return self.symptoms

	def get_illness(self):
		""" API function"""
		return self.illness


def parse_data(filepath):
	with open(filepath) as data_file:
		records = []
		for line in data_file:
			words = line.strip().split()
			records.append(Record(words[0], words[1:]))
		return records
		
		
class Diagnoser:
	def __init__(self, root):
		self.root = root


	def get_root(self):
		return self.root

	def diagnose(self, symptoms):
		""" returns leaf of decision tree whos path corresponds to list of symptoms"""
		return self.diagnose_helper(self.root, symptoms)

	def diagnose_helper(self, node, symptoms):
		"""
		helper function- traverses tree according to list of symptoms, returns relevant leaf with illness
		"""
		if node.is_leaf():
			return node.get_data()
		else:
			if node.get_data() in symptoms:
				return self.diagnose_helper(node.get_pos_child(), symptoms)
			else:
				return self.diagnose_helper(node.get_neg_child(), symptoms)

		
	def calculate_success_rate(self, records):
		""" returns rate of error for diagnosis check of records"""
		errors = 0
		for record in records:
			if self.diagnose(record.get_symptoms()) != record.illness:
				errors += 1
		return errors / len(records)


	def children(self, node, illnesses):

		""" helper function: receives specific node and dict, if node is leaf updates node.data in dict"""
		if node.is_leaf():
			if node.data in illnesses:
				illnesses[node.data] += 1
			else:
				illnesses[node.data] = 1
		else:
			self.children(node.get_pos_child(), illnesses)
			self.children(node.get_neg_child(), illnesses)


	def all_illnesses(self):

		"""
		returns all illnesses in decision tree by sorting order of most common disease.
		"""

		illnesses = {}
		self.children(self.root, illnesses)
		good_list = sorted(illnesses.items(), key=lambda x: x[1], reverse=True)
		return [z[0] for z in good_list]


	def get_symptoms(self, node):
		"""
		helper function to get all symptoms in decision tree
		:param node: any node on tree, starting with root
		:return: list of all diagnoses
		"""
		if not node.is_leaf():
			return [node.data] + self.get_symptoms(node.get_neg_child()) + self.get_symptoms(node.get_pos_child())
		return []
		
	def difference(self, other):

		""" function receives two decision trees, returns list of symptoms for which trees give different diagnoses.
		"""

		def subsets1(seq):
			"""
			helper function from class to get all subsets
			:param seq: any iterable
			:return: all subsets
			"""
			z = [[]]
			for x in seq:
				z += [y + [x] for y in z]
				return z

		both_symps = set(self.get_symptoms(self.root)) & set(self.get_symptoms(other.root))
		for a in subsets1(both_symps):
			if a:  #check only none-empty subgroups
				if self.diagnose(a) != other.diagnose(a):
					return a



	def paths_to_illness(self, illness):

		"""
		function receives illness, checks how many paths exist within tree to leaves holding illness as data
		"""

		paths = []
		def path_maker(node, path, illness):

			""" helper function: returns all possible paths, appends only those leading to correct leaves"""

			if node.is_leaf():
				if node.data == illness:
					paths.append(path)
			else:
				path_maker(node.get_pos_child(), path + [True], illness)
				path_maker(node.get_neg_child(), path + [False], illness)

		path_maker(self.root, [], illness)
		return paths


	def update_leaf(self, node, path, illness):
		""" function updates leaf corresponding to path with illness"""
		if not path:
			node.data = illness
		for p in path:
			if p:
				self.update_leaf(node.get_pos_child(), path[1:], illness)
			else:
				self.update_leaf(node.get_neg_child(), path[1:], illness)

	def convert_path_to_symps(self, node, path):
		""" having received a path comprising a list of booleans, convert the path back to the symptoms
		traversed within the tree"""
		if not path:
			return []
		for p in path:
			if p:
				return [node.data] + self.convert_path_to_symps(node.get_pos_child(), path[1:])
			else:
				return self.convert_path_to_symps(node.get_neg_child(), path[1:])



def make_tree(node, cur_depth, max_depth, symptoms):
	"""
	create tree with initially empty data for leaves. and depth size corresponding to length of symptoms list

	"""

	if cur_depth == max_depth:
		return Node(None, None, None)
	else:
		return Node(symptoms[cur_depth], make_tree(node, cur_depth + 1, max_depth, symptoms),
					make_tree(node, cur_depth + 1, max_depth, symptoms))

def symptoms_in_record(record, symptoms):
	"""
	return true if all symptoms are in record, else False
	"""

	return all(s in record.get_symptoms() for s in symptoms)


def other_symptoms_in_record(record, symptoms):
	"""
		return true if any symptoms are in record, else False
		"""

	return any((s in record.get_symptoms() for s in symptoms))

def build_tree(records, symptoms):
	""" function receives list of record objects, list of symptoms, returns root of a tree with paths to all
	possible illnesses related to symptoms from list"""
	new_tree = Diagnoser(make_tree(symptoms[0], 0, len(symptoms), symptoms))  #make tree with depth of len(symptoms)
	paths = new_tree.paths_to_illness(None)  #get all paths to leaves in new_tree
	for p in paths:
		new_tree.update_leaf(new_tree.get_root(), p, most_likely_candidate(new_tree, p, records, symptoms))
	return new_tree.get_root()


def most_likely_candidate(tree, path, records, symptoms):
	""" helper function: creates dictionary of all possible illnesses, returns illnesses with most corresponding
	appearances to symptoms"""
	candidates = {}.fromkeys([r.get_illness() for r in records], 0)
	path_symptoms = tree.convert_path_to_symps(tree.get_root(), path)  # symptoms describing path to leaf
	other_symptoms = [s for s in symptoms if s not in path_symptoms]
	for r in records:
		if symptoms_in_record(r, path_symptoms) and not other_symptoms_in_record(r, other_symptoms):
			# if all of the symptoms in path_symptoms appear in record, and none of the others do:
			candidates[r.get_illness()] += 1
	return max(candidates, key=candidates.get)


def optimal_tree(records, symptoms, depth):
	"""
	function builds trees of depth n according to n-sized subgroups of the list symptoms,
	returns root of tree with highest success-rate
	"""

	subs = combinations(symptoms, depth)  #all possible subgroups
	all_trees = sorted([Diagnoser(build_tree(records, s)) for s in subs],
					   key=lambda x: x.calculate_success_rate(records))
	return all_trees[0].get_root()  #return root of tree with lowest error percentage

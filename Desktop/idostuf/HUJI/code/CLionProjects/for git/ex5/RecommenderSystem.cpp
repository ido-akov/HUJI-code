#include "RecommenderSystem.h"


/**
	 * method to load all data from file into program.
	 * @param moviesAttributesFilePath - path of file containing movie attributes
	 * @param userRanksFilePath - path of file containing user rankings
	 * @return 0 in case of success, otherwise 1
	 */
int
RecommenderSystem::loadData(const string &moviesAttributesFilePath, const string &userRanksFilePath)
{
	if (_loadMovies(moviesAttributesFilePath) == EXIT_SUCCESS)
	{
		return _loadUsers(userRanksFilePath);
	}
	return EXIT_FAILURE;
}

/**
 * get vector with similarities of movies watched by a viewer to movie for which we are predicting
 * a rating
 * @param vec of pairs of <movie, rating>
 * @param movieToPredict for which we are predicting a rating
 * @return vector of similarities
 */
vector<double>
RecommenderSystem::_getSimilarityVector(const vector<pair<string, double>> &vec,
										const string &movieToPredict) const
{
	vector<double> simVector;
	simVector.reserve(vec.size());
	for (auto const &pair: vec)
	{
		//find least similar movie
		double sim = _similarity(_movies.at(movieToPredict), _movies.at(pair.first));
		simVector.push_back(sim);
	}
	return simVector;
}

/**
	 * load movie data into map
	 * @param moviesAttributesFilePath path of file
	 * @return 0 for success, otherwise 1
	 */
int RecommenderSystem::_loadMovies(const string &moviesAttributesFilePath)
{
	std::ifstream is(moviesAttributesFilePath);
	if (!is.is_open())
	{
		std::cerr << PATH_ERROR << moviesAttributesFilePath << std::endl;
		return EXIT_FAILURE;
	}
	string line;
	while (std::getline(is, line))
	{
		string name;
		vector<double> attributes;
		_parseMovieData(line, name, attributes);
		_movies[name] = attributes;
	}
	is.close();
	return EXIT_SUCCESS;
}

/**
	 * load user data into map
	 * @param userRanksFilePath path of file
	 * @return 0 for success, otherwise 1
	 */
int RecommenderSystem::_loadUsers(const string &userRanksFilePath)
{
	std::ifstream is(userRanksFilePath);
	if (!is.is_open())
	{
		std::cerr << PATH_ERROR << userRanksFilePath << std::endl;
		return EXIT_FAILURE;
	}
	string line;
	//parse first line
	_parseMovieOrder(is, line);
	while (std::getline(is, line))
	{
		string name;
		vector<pair<string, double>> rankings;
		_parseUserData(line, name, rankings);
		_users[name] = rankings;
	}
	is.close();
	return EXIT_SUCCESS;
}

/**
	  * parse data from file representing user database
	  * @param line of file
	  * @param name of user
	  * @param user rankings
	  */
void RecommenderSystem::_parseUserData(const string &line, string &name,
									   vector<pair<string, double>> &rankings)
{
	std::istringstream iss(line);
	iss >> name;
	string rank;
	int i = 0;
	while (iss >> rank)
	{
		double value = (rank == NO_RANK) ? 0 : stod(rank);
		string movie = _userRankingOrder.at(i);
		pair<string, double> pair(movie, value);
		rankings.push_back(pair);
		i++;
	}
	rankings.shrink_to_fit();
}

/**
	 * keep track of film order by which users rank
	 * @param is input stream
	 * @param line of user file
	 */
void RecommenderSystem::_parseMovieOrder(std::ifstream &is, string &line)
{
	std::getline(is, line);
	int i = 0;
	string movie;
	std::istringstream iss(line);
	while (iss >> movie)
	{
		_userRankingOrder[i++] = movie;
	}
}

/**
	  * parse data from file representing movie database
	  * @param line of file
	  * @param name of movie
	  * @param attributes of movie
	  */
void
RecommenderSystem::_parseMovieData(const string &line, string &name,
								   vector<double> &attributes)
{
	string values;
	std::istringstream iss(line);
	iss >> name;
	double d;
	while (iss >> d)
	{
		attributes.push_back(d);
	}
	attributes.shrink_to_fit();
	if (!_movieAttributeNum)
	{
		_movieAttributeNum = attributes.size();
	}
}

/**
	 * method recommends a user a movie according to his
	 * preferences.
	 * @param userName
	 * @return string representing the movie the customer will most enjoy based on his rankings, or
	 * error message in case user wasn't found.
	 */
string RecommenderSystem::recommendByContent(const string &userName)
{
	if (!_users.count(userName))
	{
		return USER_ERROR;
	}
	vector<double> preference = _preferenceVector(userName);
	double best = LEAST_SIMILIAR;
	string recommendation;
	for (auto const &value: _users.at(userName))
	{
		//if movie wasn't seen by user
		if (!value.second)
		{
//			compute similarity with current movie
			double sim = _similarity(preference, _movies.at(value.first));
			if (sim > best)
			{
				best = sim;
				recommendation = value.first;
			}
		}
	}
	//we know at least one movie was watched by the user, thus this will not be empty
	return recommendation;
}

/**
	 * find preference vector for a user
	 * @param name of user
	 * @return vector with preference values
	 */
vector<double> RecommenderSystem::_preferenceVector(const string &name)
{
	vector<pair<string, double>> vecCopy = _users.at(name);
	double average = _ratingsAvg(vecCopy);
	_normalize(vecCopy, average);
	//make a vector of size movie attributes with values 0. Each time add to it with transform.
	vector<double> preference(_movieAttributeNum, 0);
	for (auto const &value: vecCopy)
	{
		if (value.second)
		{
			vector<double> multiplied = _multiplyByScalar(_movies.at(value.first), value.second);
			std::transform(preference.begin(), preference.end(), multiplied.begin(), preference
					.begin(), std::plus<>());
		}
	}
	return preference;
}

/**
	 * normalize user rating values according to average
	 * @param vec of ratings
	 * @param average to normalize by
	 */
void RecommenderSystem::_normalize(vector<pair<string, double>> &vec, double average)
{
	for (auto &value: vec)
	{
		if (value.second)
		{
			value.second -= average;
		}
	}
}

/**
	 * get average of user ratings
	 * @param vec of ratings of a user
	 * @return average
	 */
double RecommenderSystem::_ratingsAvg(const vector<pair<string, double>> &vec) const
{
	double sum = 0.0;
	int i = 0;
	for (auto const &value: vec)
	{
		if (value.second)
		{
			sum += value.second;
			i++;
		}
	}
	//we know at least one movie was rated in this vector, thus i!=0;
	return sum / i;
}

/**
	 * multiply a vector by a scalar
	 * @param vector to be multiplied
	 * @param second double to multiply by
	 * @return vector with multiplied values
	 */
vector<double> RecommenderSystem::_multiplyByScalar(vector<double> &vec, double second)
{
	vector<double> result(vec);
	for (auto &value: result)
	{
		value *= second;
	}
	return result;
}

/**
	  * method to return prediction of user's ranking for a movie
	 * @param movieName
	 * @param userName
	 * @param k - parameter for CF algorithm
	 * @return prediction for user score. -1 in case of error.
	 */
double RecommenderSystem::predictMovieScoreForUser(const string &movieName, const string &userName,
												   long unsigned int k)
{
	if (!_users.count(userName) || !_movies.count(movieName) || k <= 0)
	{
		return PREDICTION_ERROR;
	}
	//collect all movies seen by user
	vector<pair<string, double>> moviesSeen;
	for (auto const &pair: _users.at(userName))
	{
		if (pair.second)
		{
			moviesSeen.push_back(pair);
		}
	}
	if (k > moviesSeen.size())
	{
		return PREDICTION_ERROR;
	}
	moviesSeen.shrink_to_fit();
	return _collaborativeFilterAlgo(moviesSeen, movieName, k);


}

/**
	 * algorithm for predicting a user's rating of a movie
	 * @param vector of movies user has seen with ratings
	 * @param basicString name of movie who's rating is to be predicted
	 * @param k number of movies to predict by
	 * @return predicted rating of movie
	 */
double
RecommenderSystem::_collaborativeFilterAlgo(vector<pair<string, double>> &vec,
											const string &movieToPredict, long unsigned int k)
{
	//filter out movies least similar to movie we are predicting
	vector<double> simVector = _getSimilarityVector(vec, movieToPredict);
	while (vec.size() > k)
	{
		pair<string, double> leastSimilar;
		double sim = MOST_SIMILIAR;
		auto iter1 = vec.begin();
		auto iter2 = simVector.begin();
		unsigned long int indexToErase = 0;
		for (unsigned long int i = 0; i < vec.size(); i++)
		{
			double cur = *(iter2 + i);
			pair<string, double> pair = *(iter1 + i);
			if (cur < sim)
			{
				leastSimilar = pair;
				sim = cur;
				indexToErase = i;
			}
		}
		vec.erase(std::remove(vec.begin(), vec.end(), leastSimilar), vec.end());
		simVector.erase(iter2 + indexToErase);
	}
	vector<double> rateVector = _getRatingVector(vec);
	return std::inner_product(simVector.begin(), simVector.end(), rateVector.begin(), 0.0) /
		   std::accumulate(simVector.begin(), simVector.end(), 0.0);
}

/**
	 * get vector only with user ratings
	 * @param vec of pairs <movie, rating>
	 * @return vector of ratings
	 */
vector<double> RecommenderSystem::_getRatingVector(const vector<pair<string, double>> &vec) const
{
	vector<double> rateVector;
	rateVector.reserve(vec.size());
	for (auto const &pair: vec)
	{
		//find least similar movie
		rateVector.push_back(pair.second);
	}
	return rateVector;
}

/**
	 * CF-based method to recommend a movie to a user
	 * @param userName
	 * @param k - parameter for CF algorithm
	 * @return name of movie in case user was found, otherwise error message
	 */
string RecommenderSystem::recommendByCF(const string &userName, long unsigned int k)
{
	if (!_users.count(userName))
	{
		return USER_ERROR;
	}
	double bestRating = 0.0;
	string movieToRecommend;
	for (auto const &pair: _users.at(userName))
	{
		if (!pair.second)
		{
			double cur = predictMovieScoreForUser(pair.first, userName, k);
			if (cur > bestRating)
			{
				bestRating = cur;
				movieToRecommend = pair.first;
			}
		}
	}
	return movieToRecommend;
}

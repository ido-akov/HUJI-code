//
// Created by Tamar Akov on 6/21/20.
//
#include <unordered_map>
#include <vector>
#include <fstream>
#include <cmath>
#include <iostream>
#include <sstream>
#include <numeric>
#include <utility>
#include <algorithm>

using std::string;
using std::unordered_map;
using std::vector;
using std::sqrt;
using std::pair;

#define PATH_ERROR "Unable to open file "
#define USER_ERROR "USER NOT FOUND"
#define PREDICTION_ERROR -1
#define NO_RANK "NA"
#ifndef EX5__RECOMMENDERSYSTEM_H
#define EX5__RECOMMENDERSYSTEM_H
#define LEAST_SIMILIAR -1.01
#define MOST_SIMILIAR 1.01

/**
 * system for recommendation of movies at "Cinema-city" theatre.

 * API:
*/

class RecommenderSystem
{
private:
	/**
	 * members:
	 * @param movies - unordered map which saves all movies and their attributes vector
	 * @param users - unordered map which saves all customers and their rankings of movies
	 * @param userRankingOrder - unordered map which keeps track of movie order for users
	 * @param movieAttributeNum - helper member to limit size of a vector
	 */
	unordered_map<string, vector<double>> _movies;
	unordered_map<string, vector<pair<string, double>>> _users;
	unordered_map<int, string> _userRankingOrder;
	size_t _movieAttributeNum;

	/**
	 * methods
	 */
	/**
	 * parse data from file representing movie database
	 * @param line of file
	 * @param name of movie
	 * @param attributes of movie
	 */
	void _parseMovieData(const string &line, string &name, vector<double> &attributes);

	/**
	 * load movie data into map
	 * @param moviesAttributesFilePath path of file
	 * @return 0 for success, otherwise 1
	 */
	int _loadMovies(const string &moviesAttributesFilePath);

	/**
	 * keep track of film order by which users rank
	 * @param is input stream
	 * @param line of user file
	 */
	void _parseMovieOrder(std::ifstream &is, string &line);

	/**
	 * load user data into map
	 * @param userRanksFilePath path of file
	 * @return 0 for success, otherwise 1
	 */
	int _loadUsers(const string &userRanksFilePath);

	/**
	  * parse data from file representing user database
	  * @param line of file
	  * @param name of user
	  * @param user rankings
	  */
	void _parseUserData(const string &line, string &name,
						vector<pair<string, double>> &rankings);

	/**
	 * method to calculate norm of a vector
	 * @param vec
	 * @return norm
	 */
	double _norm(const vector<double> &vec) const
	{ return sqrt(std::inner_product(vec.begin(), vec.end(), vec.begin(), 0.0)); }

	/**
	 * method to compute similarity between two vectors
	 * @param first vector
	 * @param second vector
	 * @return similarity- between -1 and 1
	 */
	double _similarity(const vector<double> &first, const vector<double> &second) const
	{
		return std::inner_product(first.begin(), first.end(), second.begin(), 0.0) /
			   (_norm(first) * _norm(second));
	}

	/**
	 * find preference vector for a user
	 * @param name of user
	 * @return vector with preference values
	 */
	vector<double> _preferenceVector(const string &name);

	/**
	 * get average of user ratings
	 * @param vec of ratings of a user
	 * @return average
	 */
	double _ratingsAvg(const vector<pair<string, double>> &vec) const;

	/**
	 * normalize user rating values according to average
	 * @param vec of ratings
	 * @param average to normalize by
	 */
	void _normalize(vector<pair<string, double>> &vec, double average);

	/**
	 * multiply a vector by a scalar
	 * @param vector to be multiplied
	 * @param second double to multiply by
	 * @return vector with multiplied values
	 */
	vector<double> _multiplyByScalar(vector<double> &vector, double second);

	/**
	 * algorithm for predicting a user's rating of a movie
	 * @param vector of movies user has seen with ratings
	 * @param basicString name of movie who's rating is to be predicted
	 * @param k number of movies to predict by
	 * @return predicted rating of movie
	 */
	double _collaborativeFilterAlgo(vector<pair<string, double>> &vector,
									const string &basicString, long unsigned int k);

	/**
	 * get vector only with user ratings
	 * @param vec of pairs <movie, rating>
	 * @return vector of ratings
	 */
	vector<double> _getRatingVector(const vector<pair<string, double>> &vec) const;

	/**
	 * get vector with similarities of movies watched by a viewer to movie for which we are predicting
	 * a rating
	 * @param vec of pairs of <movie, rating>
	 * @param movieToPredict for which we are predicting a rating
	 * @return vector of similarities
	 */
	vector<double> _getSimilarityVector(const vector<pair<string, double>> &vec,
										const string &movieToPredict) const;

public:

	/**
	 * method to load all data from file into program.
	 * @param moviesAttributesFilePath - path of file containing movie attributes
	 * @param userRanksFilePath - path of file containing user rankings
	 * @return 0 in case of success, otherwise 1
	 */
	int loadData(const string &moviesAttributesFilePath, const string &userRanksFilePath);

	/**
	 * method recommends a user a movie according to his
	 * preferences.
	 * @param userName
	 * @return string representing the movie the customer will most enjoy based on his rankings, or
	 * error message in case user wasn't found.
	 */
	string recommendByContent(const string &userName);

	/**
	  * method to return prediction of user's ranking for a movie
	 * @param movieName
	 * @param userName
	 * @param k - parameter for CF algorithm
	 * @return prediction for user score. -1 in case of error.
	 */
	double predictMovieScoreForUser(const string &movieName, const string &userName,
									long unsigned int k);

	/**
	 * CF-based method to recommend a movie to a user
	 * @param userName
	 * @param k - parameter for CF algorithm
	 * @return name of movie in case user was found, otherwise error message
	 */
	string recommendByCF(const string &userName, long unsigned int k);

};

#endif //EX5__RECOMMENDERSYSTEM_H

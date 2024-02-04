//
// Created on 2/20/2022.
//

#ifndef RECOMMENDATIONSYSTEM_H
#define RECOMMENDATIONSYSTEM_H
#include <map>
#include <numeric>
#include <algorithm>
#include <utility>
#include "User.h"

struct movie_comparator
{
    bool operator() (const sp_movie &movie1,
            const sp_movie &movie2) const
    {
      // Compare the underlying Movie objects
      if (movie1 && movie2) // Avoid dereference Null values.
      {
        return (*movie1) < (*movie2);
      }
      return !movie1 && movie2;
    }
};

typedef std::map<sp_movie, std::vector<double>,
movie_comparator> data_base;

class RecommendationSystem
{
 private:
  data_base _data_base;
  /**
   * Order user unseen movies by similarity
   * @param user to to order the similarity by
   * @param sp_movie movie to order the similarity by
   * @param k size to keep
   * @return vector[pair(sp_movie,grade)] order by similarity
   */
  std::vector<std::pair<sp_movie, double>>
  order_by_similarity (const User &user, const sp_movie &movie, int k);

    /**
     * Makes a user a preference vector for SEEN movies
     * @param user to to order the similarity by
     * @return vector - every cell represent the feature
     * effect on the user (high = like)
     */
  std::vector<double> get_preferences (const User &user);

    /**
     * Finds the user the best movie he haven't seen
     * (by first module)
     * @param preferences to to order the similarity by
     * @param user to to order the similarity by
     * @return A movie with highest similarity to user
     */
  sp_movie
  find_best_fit (std::vector<double> preferences,
                 const User &user) const;

    /**
    * Compute avg effect for all the features of one movie.
    * @param user_rank map of movies rank
    * @return the avg.
    */
  double avg (const rank_map &user_rank) const;

 public:
  explicit RecommendationSystem (){};
  /**
   * adds a new movie to the system
   * @param name name of movie
   * @param year year it was made
   * @param features features for movie
   * @return shared pointer for movie in system
   */
  sp_movie
  add_movie (const std::string &name, int year,
             const std::vector<double> &features);

  /**
   * a function that calculates the movie with highest
   * score based on movie features
   * @param ranks user ranking to use for algorithm
   * @return shared pointer to movie in system
   */
  sp_movie recommend_by_content (const User &user);

  /**
   * a function that calculates the movie with highest
   * predicted score based on ranking of other movies
   * @param ranks user ranking to use for algorithm
   * @param k
   * @return shared pointer to movie in system
   */
  sp_movie recommend_by_cf (const User &user, int k);

  /**
   * Predict a user rating for a movie given argument using
   * item cf procedure with k most similar movies.
   * @param user_rankings: ranking to use
   * @param movie: movie to predict
   * @param k:
   * @return score based on algorithm as described in pdf
   */
  double predict_movie_score
  (const User &user, const sp_movie &movie, int k);

  /**
   * gets a shared pointer to movie in system
   * @param name name of movie
   * @param year year movie was made
   * @return shared pointer to movie in system
   */
  sp_movie get_movie (const std::string &name, int year) const;

  friend std::ostream &
  operator<< (std::ostream &os, const RecommendationSystem &rs);
};

#endif //RECOMMENDATIONSYSTEM_H

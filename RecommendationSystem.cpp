//
// Created by יהונתן זלטין on 25/06/2023.
//
#include "RecommendationSystem.h"
#include <cmath>

sp_movie
RecommendationSystem::add_movie (const std::string &name,
                                 int year, const std::vector<double> &features)
{
  sp_movie sp_movie1 = std::make_shared<Movie> (name, year);
  _data_base[sp_movie1] = features;
  return sp_movie1;
}

bool compute_by_similarity(const std::pair<sp_movie,double>& m1,
                           const std::pair<sp_movie,double>& m2)
{
    // On base of their similarities
    return m1.second > m2.second;
}

double
similarity (const std::vector<double> preferences,
            const std::vector<double> movie)
{
  double numerator = 0;
  double norm1 = 0;
  double norm2 = 0;
  for(int i =0; i < (int)preferences.size(); i++)
  {
      numerator += preferences[i] * movie[i];
      norm1 += preferences[i] * preferences[i];
      norm2 += movie[i] * movie[i];
  }
  return numerator/(std::sqrt(norm1)*std::sqrt(norm2));
}

std::vector<std::pair<sp_movie, double>>
RecommendationSystem::order_by_similarity (const User &user,
                                           const sp_movie &movie, int k)
{
  rank_map user_rank = user.get_ranks ();
  std::vector<std::pair<sp_movie, double>> ordered_ranks;

  for (auto movie_pair: _data_base)
  {
    sp_movie it_movie = movie_pair.first;
    if (user_rank.find (it_movie) != user_rank.end ()) //only seen movies
    {
      double c_similarity = similarity (_data_base[movie],
                                        _data_base[it_movie]);
      ordered_ranks.emplace_back (it_movie, c_similarity);
    }
  }

  std::sort (ordered_ranks.begin (), ordered_ranks.end (),
             compute_by_similarity);
  if ((int)ordered_ranks.size () < k)
  {
    return ordered_ranks;
  }
  ordered_ranks.erase (ordered_ranks.begin () + k,
                       ordered_ranks.end ());
  return ordered_ranks;
}

double RecommendationSystem::avg (const rank_map &user_rank) const
{
  double avg = 0;
  for (std::pair<sp_movie, double> entry: user_rank)
  {
    avg += entry.second;
  }
  return avg / user_rank.size ();
}

std::vector<double>
RecommendationSystem::get_preferences (const User &user)
{
  rank_map watched_movies = user.get_ranks ();
  int rank_size = _data_base.begin()->second.size ();
  double avg_rank = avg (watched_movies);

  std::vector<double> preferences (rank_size, 0);
  for (std::pair<sp_movie, std::vector<double>> movie_pair: _data_base)
  {
    if (watched_movies.find (movie_pair.first) !=
    watched_movies.end ()) //check only seen movies
    {
        const std::vector<double> &feature_values = movie_pair.second;
        int i = 0;
        for (double f: feature_values)
        {
            preferences[i++] +=
                    f * (watched_movies[movie_pair.first] - avg_rank);
        }
    }
  }
  return preferences;
}

sp_movie
RecommendationSystem::find_best_fit (std::vector<double> preferences,
                                     const User &user) const
{
  rank_map user_rank = user.get_ranks ();
  // Initiate default values for the best fit
  double best_fit_score =
          similarity(preferences,_data_base.begin()->second);
  sp_movie best_fit_movie = _data_base.begin()->first;

  for (std::pair<sp_movie, std::vector<double>> movie_pair: _data_base)
  {
    if (user_rank.find (movie_pair.first) == user_rank.end ())
        // check only unseen movies
    {
      double c_similarity =
              similarity (preferences, movie_pair.second);
      if (c_similarity > best_fit_score)
      {
        best_fit_score = c_similarity;
        best_fit_movie = movie_pair.first;
      }
    }
  }
  return best_fit_movie;
}

sp_movie RecommendationSystem::
recommend_by_content (const User &user)
{
  std::vector<double> preferences = get_preferences (user);
  return find_best_fit (preferences, user);
}

sp_movie RecommendationSystem::
recommend_by_cf (const User &user, int k)
{
  // Initiate default values for the best fit
  double best_fit_score = 0;
  sp_movie best_fit_movie = nullptr;
  rank_map watched_movies = user.get_ranks ();
  for (auto movie_pair: _data_base)
  {
    sp_movie c_sp_movie = movie_pair.first;
    if (watched_movies.find (c_sp_movie) == watched_movies.end ())
        // only unseen movies
    {
      double c_score = predict_movie_score (user, c_sp_movie, k);
      if (c_score > best_fit_score)
      {
        best_fit_score = c_score;
        best_fit_movie = c_sp_movie;
      }
    }
  }
  return best_fit_movie;
}

double RecommendationSystem::
predict_movie_score (const User &user,const sp_movie &movie, int k)
{
  rank_map watched_movies = user.get_ranks ();
  // ordering seen movies with "movie" an unseen one
  auto ranks_ordered = order_by_similarity (user, movie, k);
  double numerator = 0;
  double denominator = 0;
  for (auto v: ranks_ordered)
  {
    numerator += (v.second) * watched_movies[v.first];
    denominator += v.second;
  }
  return numerator / denominator;
}

sp_movie RecommendationSystem::get_movie
(const std::string &name, int year) const
{
  sp_movie movie2find = std::make_shared<Movie> (name, year);
  const auto it = _data_base.find (movie2find);
  if (it == _data_base.end ())
  {
    return nullptr;
  }
  return it->first;
}

std::ostream &operator<< (std::ostream &os,
        const RecommendationSystem &rs)
{
  std::vector<sp_movie> sorted_movies;
  sorted_movies.reserve (rs._data_base.size ());

  for (const auto &movie_pair: rs._data_base)
  {
    sorted_movies.push_back (movie_pair.first);
  }

  std::sort (sorted_movies.begin (),
             sorted_movies.end (), movie_comparator ());

  for (const auto &movie: sorted_movies)
  {
    os << *movie << std::endl;
  }
  return os;
}
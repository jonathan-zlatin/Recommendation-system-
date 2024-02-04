

// don't change those includes
#include "User.h"
#include "RecommendationSystem.h"


// implement your cpp code here

void User::add_movie_to_rs (const std::string &name, int year,
                            const std::vector<double> &features,
                            double rate)
{
  sp_movie new_movie = _rsm->add_movie (name, year, features);
  _rank_map[new_movie] = rate;
}

sp_movie User::get_recommendation_by_content () const
{
  return _rsm->recommend_by_content (*this);
}

sp_movie User::get_recommendation_by_cf (int k) const
{
  return _rsm->recommend_by_cf (*this, k);
}

double User::get_prediction_score_for_movie
(const std::string &name, int year, int k) const
{
  sp_movie new_movie = _rsm->get_movie(name, year);
  double score = _rsm->predict_movie_score
          (*this, new_movie, k);
  return score;
}

std::ostream &operator<< (std::ostream &os, User &user)
{
  os << "name:" << user._user_name << "\n";
  os << (*user._rsm);
  os << std::endl;

  return os;
}
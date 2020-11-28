#define CGAL_EIGEN3_ENABLED
#include <CGAL/Epick_d.h>
#include <CGAL/point_generators_d.h>
#include <CGAL/Manhattan_distance_iso_box_point.h>
#include <CGAL/K_neighbor_search.h>
#include <CGAL/Search_traits_d.h>
#include <vector>
#include <utility>
typedef CGAL::Epick_d<CGAL::Dimension_tag<1089> > Kernel;
typedef Kernel::Point_d Point_d;
typedef CGAL::Random_points_in_cube_d<Point_d> Random_points_iterator;
typedef Kernel::Iso_box_d Iso_box_d;
typedef Kernel TreeTraits;
typedef CGAL::Manhattan_distance_iso_box_point<TreeTraits> Distance;
typedef CGAL::K_neighbor_search<TreeTraits, Distance> Neighbor_search;
typedef Neighbor_search::Tree Tree;
int  main() {
  const int N = 1000;
  Tree tree;
  Random_points_iterator rpit(1089,1000.0);
  std::vector<std::pair<std::string,std::vector<double>>> points;
  for(int i=0; i<points.size(); i++){
    Point_d dd(1089, points[i].second.begin(), points[i].second.end());
    tree.insert(dd);
  }
  std::vector<std::vector<std::pair<std::string,std::vector<double>>>> test;
  for(int j=0; j<test.size(); j++){
    for(int i=0; i<test[j].size(); i++){
      Point_d pp(1089, test[j][i].second.begin(), test[j][i].second.end());
      Iso_box_d query(pp,pp);
      Distance tr_dist;
      Neighbor_search N1(tree, query, 5, 10.0, false);
      for (Neighbor_search::iterator it = N1.begin();it != N1.end();it++) {
        std::cout << " Point " << it->first << " at distance  " << tr_dist.inverse_of_transformed_distance(it->second) << std::endl;
      }
    }
  }
  return 0;
}
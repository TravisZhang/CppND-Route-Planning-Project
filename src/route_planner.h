#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "route_model.h"


class RoutePlanner {
  public:
    RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y);
    // Add public variables or methods declarations here.

    float GetDistance() const { return distance; }

    void AStarSearch();

  private:
    // Add private variables or methods declarations here.
    RouteModel &m_Model;
    RouteModel::Node *start_node, *end_node; // start and end nodes closest to selected points
    float distance; //  the total distance for the route that A* search finds from start_node to end_node
    std::vector<RouteModel::Node*> open_list; // list of nodes not yet evaluated

    std::vector<RouteModel::Node> ConstructFinalPath(RouteModel::Node *current_node);

    float CalculateHValue(const RouteModel::Node *node);

    RouteModel::Node *NextNode(); // find the lowest f value node to be the next evaluating node

    void AddNeighbors(RouteModel::Node *current_node); // add available neighbors of the current node to open list

    class CompareFValue {
    public:
      bool operator() (RouteModel::Node *a, RouteModel::Node *b) { 
        return (a->g_value+a->h_value < b->g_value+b->h_value);
      }
    };
};

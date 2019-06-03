#include "route_model.h"
#include <iostream>

RouteModel::RouteModel(const std::vector<std::byte> &xml) : Model(xml) {
    int cnt = 0;
    for(auto &node: this->Nodes())
    {
        m_Nodes.push_back(Node(cnt, this, node));
        ++cnt;
    }

    CreateNodeToRoadHashmap();
}

void RouteModel::CreateNodeToRoadHashmap() {
    for(auto &road: this->Roads())
    {
        if(road.type != Model::Road::Type::Footway)
        {
            for(auto &node_idx: Ways()[road.way].nodes)
            {
                // if the node is not yet added with current road
                if(node_to_road.find(node_idx) == node_to_road.end())
                {
                    node_to_road[node_idx] = std::vector<const Model::Road*>();
                }
                // add current road to its belonging node
                node_to_road[node_idx].push_back(&road);
            }
        }
    }
}

RouteModel::Node *RouteModel::Node::FindNeighbor(const std::vector<int> &node_indices) {
    Node *closest_node = nullptr;
    for(auto node_idx: node_indices)
    {
        Node *node = &parent_model->SNodes()[node_idx];
        auto dist = this->distance(*node);
        if(dist > 0 && !node->visited)
        {
            if(closest_node == nullptr || dist < this->distance(*closest_node))
            {
                // closest_node = &node;
                // closest_node = &(parent_model->SNodes()[node_idx]);
                closest_node = node;
            }
        }
    }
    return closest_node;
}

void RouteModel::Node::FindNeighbors() {
    for(auto &road: parent_model->node_to_road[this->index])
    {
        Node *closest_node = FindNeighbor(parent_model->Ways()[road->way].nodes);
        if(closest_node)
        {
            this->neighbors.push_back(closest_node);
        }
    }
}

RouteModel::Node& RouteModel::FindClosestNode(float x, float y) {
    RouteModel::Node node_c;
    node_c.x = x;
    node_c.y = y;
    float min_dist = std::numeric_limits<float>::max();
    int closest_idx;
    for(auto &road: Roads())
    {
        if(road.type != Model::Road::Type::Footway)
        {
            for(auto &node_idx: Ways()[road.way].nodes)
            {
                float dist = SNodes()[node_idx].distance(node_c);
                if(min_dist > dist)
                {
                    min_dist = dist;
                    closest_idx = node_idx;
                }
            }
        }
    }
    return SNodes()[closest_idx];
}
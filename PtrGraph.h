#pragma once

#include <vector>
#include <iostream>
#include <ostream>

namespace ad {
    template <typename T>
    class PtrGraph {
    private:
        //邻接表结点
        struct GNode {
            T* p_data; //结点指针
            std::vector<int> out; //自己指向的结点（下标）
            std::vector<int> in; //指向自己的结点（下标）
            explicit GNode(T* d = nullptr): p_data(d) { }
        };

        std::vector<GNode> adjacency_table_; //邻接表(有向)
        int e_;

    public:
        PtrGraph(): adjacency_table_(), e_(0) {
            adjacency_table_.reserve(100);
        }
        PtrGraph(const PtrGraph& other) = default;
        PtrGraph(PtrGraph&& other) = default;

        PtrGraph& operator=(const PtrGraph& other) = default;
        PtrGraph& operator=(PtrGraph&& other) = default;

        ~PtrGraph() {
            for (const auto& item : adjacency_table_) {
                delete item.p_data;
            }
        }

        T& operator[](int idx) {
            return *adjacency_table_[idx].p_data;
        }

        T& GetIn(int self, int child) {
            return *adjacency_table_[adjacency_table_[self].in[child]].p_data;
        }

        const std::vector<int>& GetIns(int self) {
            return adjacency_table_[self].in;
        }

        T& GetOut(int self, int parent) {
            return *adjacency_table_[adjacency_table_[self].out[parent]].p_data;
        }

        const std::vector<GNode>& GetOuts(int self) {
            return adjacency_table_[self].out;
        }

        int AddNode(T* p_data) {
            int idx; //如果p_data在图中直接返回下标
            if ((idx = In(p_data)) != -1) {
                return idx;
            }
            idx = adjacency_table_.size();
            adjacency_table_.emplace_back(p_data);
            return idx; //返回下标
        }

        void AddEdge(T* p_head, T* p_tail) {
            int head_idx = AddNode(p_head);
            int tail_idx = AddNode(p_tail);
            for (const auto& arrow : adjacency_table_[head_idx].out) {
                if (arrow == tail_idx) return;
            }
            e_++;
            adjacency_table_[head_idx].out.push_back(tail_idx);
            adjacency_table_[tail_idx].in.push_back(head_idx);
        }

        friend std::ostream& operator <<(std::ostream& os, const PtrGraph& obj) {
            os << "adjacency_table: \n";
            for (const auto& node : obj.adjacency_table_) {
                os << *node.p_data << " ";
                for (auto idx : node.out) {
                    os << "->" << *obj.adjacency_table_[idx].p_data;
                }
                os << std::endl;
            }
            os << "inverse adjacency_table: \n";
            for (const auto& node : obj.adjacency_table_) {
                os << *node.p_data << " ";
                for (auto idx : node.in) {
                    os << "<-" << *obj.adjacency_table_[idx].p_data;
                }
                os << std::endl;
            }

            os << "len of graph: " << obj.V();
            return os;
        }

        //判断p_data是否在图中
        int In(T* p_data) const {
            for (size_t idx = 0; idx < adjacency_table_.size(); idx++) {
                if (adjacency_table_[idx].p_data == p_data) return static_cast<int>(idx);
            }
            return -1;
        }

        int V() const {
            return adjacency_table_.size();
        }

        int E() const {
            return e_;
        }

        // void BFS();
        // void DFS();
    };
}

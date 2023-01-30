#pragma once
#include <vector>
#include <string>
#include <cmath>
#include <ostream>
#include "pointer_graph.h"

namespace ad {
    enum Op { ADD, MIN, MUL, DIV, SIN, COS, LOG, NONE };

    class BackwardModeTensor {
    public:
        //隐式存放计算图
        static PtrGraph<BackwardModeTensor> compute_graph_;

        friend std::ostream& operator<<(std::ostream& os, const BackwardModeTensor& obj) {
            return os
                << "("
                << "index: " << obj.index_
                << " value: " << obj.value_
                << " grad: " << obj.grad_
                << " o: " << obj.o_
                << ")";
        }

        int index_;
        double value_;
        double grad_;
        Op o_;

        explicit BackwardModeTensor(const double value = 0, const Op& o = Op::NONE)
            : index_(-1), value_(value), grad_(0), o_(o) { }

        void ComputeValue() {
            SetIndex();

            switch (o_) {
            case Op::ADD:
                Add();
                break;
            case Op::MIN:
                Min();
                break;
            case Op::MUL:
                Mul();
                break;
            case Op::DIV:
                Div();
                break;
            case Op::SIN:
                Sin();
                break;
            case Op::COS:
                Cos();
                break;
            case Op::LOG:
                Log();
                break;
            case Op::NONE:
                break;
            }
        }

        void SetIndex() {
            index_ = compute_graph_.In(this);
        }


        BackwardModeTensor& Parent(const int c) const {
            return compute_graph_.GetIn(index_, c);
        }

        BackwardModeTensor& Child(const int p) const {
            return compute_graph_.GetOut(index_, p);
        }

        BackwardModeTensor& operator+(BackwardModeTensor& other) const {
            const auto new_tensor = new BackwardModeTensor(0, Op::ADD);
            compute_graph_.AddEdge(&compute_graph_[index_], new_tensor);
            compute_graph_.AddEdge(&other, new_tensor);
            new_tensor->ComputeValue();
            return *new_tensor;
        }

        BackwardModeTensor& operator-(BackwardModeTensor& other) const {
            const auto new_tensor = new BackwardModeTensor(0, Op::MIN);
            compute_graph_.AddEdge(&compute_graph_[index_], new_tensor);
            compute_graph_.AddEdge(&other, new_tensor);
            new_tensor->ComputeValue();
            return *new_tensor;
        }

        BackwardModeTensor& operator*(BackwardModeTensor& other) const {
            const auto new_tensor = new BackwardModeTensor(0, Op::MUL);
            compute_graph_.AddEdge(&compute_graph_[index_], new_tensor);
            compute_graph_.AddEdge(&other, new_tensor);
            new_tensor->ComputeValue();
            return *new_tensor;
        }

        BackwardModeTensor& operator/(BackwardModeTensor& other) const {
            const auto new_tensor = new BackwardModeTensor(0, Op::DIV);
            compute_graph_.AddEdge(&compute_graph_[index_], new_tensor);
            compute_graph_.AddEdge(&other, new_tensor);
            new_tensor->ComputeValue();
            return *new_tensor;
        }

        void Backward() {
            grad_ = 1;
            ComputeGrad();
        }


        void ComputeGrad() const {
            //计算下一层的梯度
            switch (o_) {
            case Op::ADD:
                GradOfAdd();
                break;
            case Op::MIN:
                GradOfMin();
                break;
            case Op::MUL:
                GradOfMul();
                break;
            case Op::DIV:
                GradOfDiv();
                break;
            case Op::SIN:
                GradOfSin();
                break;
            case Op::COS:
                GradOfCos();
                break;
            case Op::LOG:
                GradOfLog();
                break;
            case Op::NONE:
                break;
            }
            //继续向下算
            for (const int t : compute_graph_.GetIns(index_)) {
                compute_graph_[t].ComputeGrad();
            }
        }


        void Add() {
            value_ = Parent(0).value_ + Parent(1).value_;
        }

        void Min() {
            value_ = Parent(0).value_ - Parent(1).value_;
        }

        void Mul() {
            value_ = Parent(0).value_ * Parent(1).value_;
        }

        void Div() {
            value_ = Parent(0).value_ / Parent(1).value_;
        }

        void Sin() {
            value_ = std::sin(Parent(0).value_);
        }

        void Cos() {
            value_ = std::cos(Parent(0).value_);
        }

        void Log() {
            value_ = std::log(Parent(0).value_);
        }

        void GradOfAdd() const {
            Parent(0).grad_ += grad_ * 1;
            Parent(1).grad_ += grad_ * 1;
        }

        void GradOfMin() const {
            Parent(0).grad_ += grad_ * 1;
            Parent(1).grad_ += grad_ * -1;
        }

        void GradOfMul() const {
            Parent(0).grad_ += grad_ * Parent(1).value_;
            Parent(1).grad_ += grad_ * Parent(0).value_;
        }

        void GradOfDiv() const {
            Parent(0).grad_ += grad_ / Parent(1).value_;
            Parent(1).grad_ += -grad_ * Parent(0).value_
                / std::pow(Parent(1).value_, 2);
        }

        void GradOfSin() const {
            Parent(0).grad_ += grad_ * std::cos(Parent(0).value_);
        }

        void GradOfCos() const {
            Parent(0).grad_ += -grad_ * std::sin(Parent(0).value_);
        }

        void GradOfLog() const {
            Parent(0).grad_ += grad_ / Parent(0).value_;
        }
    };

    PtrGraph<BackwardModeTensor> BackwardModeTensor::compute_graph_;

    inline BackwardModeTensor& sin(BackwardModeTensor& other) {
        const auto new_tensor = new BackwardModeTensor(0, Op::SIN);
        BackwardModeTensor::compute_graph_.AddEdge(&other, new_tensor);
        new_tensor->ComputeValue();
        return *new_tensor;
    }

    inline BackwardModeTensor& cos(BackwardModeTensor& other) {
        const auto new_tensor = new BackwardModeTensor(0, Op::COS);
        BackwardModeTensor::compute_graph_.AddEdge(&other, new_tensor);
        new_tensor->ComputeValue();
        return *new_tensor;
    }

    inline BackwardModeTensor& ln(BackwardModeTensor& other) {
        const auto new_tensor = new BackwardModeTensor(0, Op::LOG);
        BackwardModeTensor::compute_graph_.AddEdge(&other, new_tensor);
        new_tensor->ComputeValue();
        return *new_tensor;
    }

    inline BackwardModeTensor& tensor(const double var) {
        const auto new_tensor = new BackwardModeTensor(var);
        BackwardModeTensor::compute_graph_.AddNode(new_tensor);
        new_tensor->SetIndex();
        return *new_tensor;
    }
    using Tensor = BackwardModeTensor&;
}

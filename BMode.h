#pragma once
#include <vector>
#include <string>
#include <cmath>
#include <ostream>
#include <memory>
#include <iostream>
#include "PtrGraph.h"

namespace ad {
    enum Op { Add, Min, Mul, Div, Sin, Cos, Log, None };

    class Tensor {
    public:
        //隐式存放计算图
        static PtrGraph<Tensor> compute_graph_;

        friend std::ostream& operator<<(std::ostream& os, const Tensor& obj) {
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

        explicit Tensor(const double value = 0, const Op& o = Op::None)
            : index_(-1), value_(value), grad_(0), o_(o) { }

        void ComputeValue() {
            SetIndex();

            switch (o_) {
            case Op::Add:
                Add();
                break;
            case Op::Min:
                Min();
                break;
            case Op::Mul:
                Mul();
                break;
            case Op::Div:
                Div();
                break;
            case Op::Sin:
                Sin();
                break;
            case Op::Cos:
                Cos();
                break;
            case Op::Log:
                Log();
                break;
            case Op::None:
                break;
            }
        }

        void SetIndex() {
            index_ = compute_graph_.In(this);
        }


        Tensor& Parent(const int c) const {
            return compute_graph_.GetIn(index_, c);
        }

        Tensor& Child(const int p) const {
            return compute_graph_.GetOut(index_, p);
        }

        Tensor& operator+(Tensor& other) const {
            const auto new_tensor = new Tensor(0, Op::Add);
            compute_graph_.AddEdge(&compute_graph_[index_], new_tensor);
            compute_graph_.AddEdge(&other, new_tensor);
            new_tensor->ComputeValue();
            return *new_tensor;
        }

        Tensor& operator-(Tensor& other) const {
            const auto new_tensor = new Tensor(0, Op::Min);
            compute_graph_.AddEdge(&compute_graph_[index_], new_tensor);
            compute_graph_.AddEdge(&other, new_tensor);
            new_tensor->ComputeValue();
            return *new_tensor;
        }

        Tensor& operator*(Tensor& other) const {
            const auto new_tensor = new Tensor(0, Op::Mul);
            compute_graph_.AddEdge(&compute_graph_[index_], new_tensor);
            compute_graph_.AddEdge(&other, new_tensor);
            new_tensor->ComputeValue();
            return *new_tensor;
        }

        Tensor& operator/(Tensor& other) const {
            const auto new_tensor = new Tensor(0, Op::Div);
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
            case Op::Add:
                GradOfAdd();
                break;
            case Op::Min:
                GradOfMin();
                break;
            case Op::Mul:
                GradOfMul();
                break;
            case Op::Div:
                GradOfDiv();
                break;
            case Op::Sin:
                GradOfSin();
                break;
            case Op::Cos:
                GradOfCos();
                break;
            case Op::Log:
                GradOfLog();
                break;
            case Op::None:
                break;
            }
            //继续向下算
            for (const int& t : compute_graph_.GetIns(index_)) {
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

    PtrGraph<Tensor> Tensor::compute_graph_;

    inline Tensor& sin(Tensor& other) {
        const auto new_tensor = new Tensor(0, Op::Sin);
        Tensor::compute_graph_.AddEdge(&other, new_tensor);
        new_tensor->ComputeValue();
        return *new_tensor;
    }

    inline Tensor& cos(Tensor& other) {
        const auto new_tensor = new Tensor(0, Op::Cos);
        Tensor::compute_graph_.AddEdge(&other, new_tensor);
        new_tensor->ComputeValue();
        return *new_tensor;
    }

    inline Tensor& ln(Tensor& other) {
        const auto new_tensor = new Tensor(0, Op::Log);
        Tensor::compute_graph_.AddEdge(&other, new_tensor);
        new_tensor->ComputeValue();
        return *new_tensor;
    }

    inline Tensor& tensor(const double var) {
        const auto new_tensor = new Tensor(var);
        Tensor::compute_graph_.AddNode(new_tensor);
        new_tensor->SetIndex();
        return *new_tensor;
    }
}

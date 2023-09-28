#ifndef SOLUTION_H
#define SOLUTION_H

#include "omega.h"
#include "digraph.h"
#include "dataloader.h"
#include "architecture.h"
#include <random>
#include <chrono>

#define MAX_TEMPERATURE 0.001
#define MIN_TEMPERATURE 0.0001
#define DECAY 0.99999

const long long int MAX_WORD = (1LL << 20) - 1;  // 20 bits
const long long int MAX_SOURCE = (1LL << 8) - 1; // 8 bits
const long long int MAX_TARGET = (1LL << 8) - 1; // 8 bits
const long long int MAX_V = 1;                   // 1 bit

/* Add Do/Undo queue for optimization */
template<typename T>
class MyStack{
public:
    /* Constructors and Destructor */
    MyStack(unsigned int size):
            mSize(size),
            mCursor(0)
    {
        mData = new T[mSize];
    }
    MyStack() = default;
    MyStack(const MyStack<T>&other){
        mData = nullptr;
        *this = other;
    }
    MyStack<T> &operator=(const MyStack<T>&other){
        if(this == &other)
            return *this;
        delete[] mData;
        mSize = other.mSize;
        mCursor = other.mCursor;
        mData = new T[mSize];
        for(int i=0; i<mSize; i++)
            mData[i] = other.mData[i];
        return *this;
    }
    ~MyStack(){
        delete[] mData;
    }
    void push(T item){
        if(mCursor >= mSize){
            std::cerr << "The maximum stack size has been exceeded.\n";
            exit(EXIT_FAILURE);
        }
        mData[ mCursor++ ] = item;
    }
    void pop(){ mCursor = ( mCursor==0 ? 0 : mCursor-1 ); }
    T top() const{ return ( mCursor==0 ? T() : mData[mCursor-1] ); }
    bool empty() const{ return mCursor==0; }
    void clear(){ mCursor = 0; }

private:
    T *mData;
    int mSize;
    int mCursor;
};

class Solution {
private: 
    Omega net;
    Digraph G;
    Architecture arc;
    MyStack<int64_t> stack;

    int _swap_two_nodes(vector<int>&solution, int cost, int node_i, int node_j);
    int _remove_node_from_pe(const vector<int>&solution, int node, int pe);
    int _add_node_into_pe(const vector<int>&solution, int node, int pe);
    void _undo_swap();

    int _route(int peu, int pev);

    double acceptance_probability(int deltaC, double temp) const;

public:
    vector<vector<int>>visited;
    // Constructor
    Solution(const string&json_file, const string&dot_file, int copy, int extra);

    // Methods 
    int evaluate_initial_solution(const vector<int>&solution);
    int greedy(vector<int>&solution, const string&search="");
    int local_search(vector<int>&solution, int cost);
    int annealing(vector<int>&solution, int cost, const vector<double>&random_values, const vector<int>&random_nodes);

    int get_optimum() const;
    int get_number_of_nodes() const{ return G.number_of_nodes(); }
    void info() const;
    int size() const;
    void clear();
};

#endif
#include<bits/stdc++.h>
using namespace std;

template <typename T>
bool min_q(T a, T b) {
    return (a<b);
}

template <typename T>
class binary_heap {
    public:
        binary_heap();
        binary_heap(vector <T> a, bool(*comparator)(T,T)=min_q);
        binary_heap(T a[], int n, bool(*comparator)(T,T)=min_q);
        ~binary_heap();
        void insert(T x, bool(*comparator)(T,T)=min_q);
        void pop(bool(*comparator)(T,T)=min_q);
        T top();
        bool empty();
        void print();
        int size();
    
    private:
        vector <T> *v;
        void fix_up(int n, bool(*comparator)(T,T)=min_q);
        void fix_down(int n, bool(*comparator)(T,T)=min_q);
        void make(bool(*comparator)(T,T)=min_q);
        void print(int i, bool left, int ht);

};

template <typename T>
binary_heap<T>::binary_heap() {
    v = new vector <T>;
}

template <typename T>
binary_heap<T>::binary_heap(vector <T> a, bool(*comparator)(T,T)) {
    v = new vector <T>;
    for(int i=0;i<a.size();i++) {
        v -> push_back(a[i]);
    }
    make(comparator);
}

template <typename T>
binary_heap<T>::binary_heap(T a[], int n, bool(*comparator)(T,T)) {
    v = new vector <T>;
    for(int i=0;i<n;i++) {
        v -> push_back(a[i]);
    }
    make(comparator);
}

template <typename T>
binary_heap<T>::~binary_heap() {
    delete v;
}

template <typename T>
void binary_heap<T>::fix_up(int n, bool(*comparator)(T,T)){
    int a = (n-1)/2;
    if(n==0) {
        return;
    }
    int c=a;
    if((2*a+1)<v->size()&&comparator(v->at(2*a+1),v->at(c)))
        c = 2*a+1;
    if((2*a+2)<v->size()&&comparator(v->at(2*a+2),v->at(c)))
        c = 2*a+2;
    if(c!=a){
        T t = v->at(a);
        v->at(a) = v->at(c);
        v->at(c) = t;
        fix_up(a, comparator);
    }    
}

template <typename T>
void binary_heap<T>::fix_down(int n, bool(*comparator)(T,T)) {
    if(n>=v->size())
        return;
    int c=n;
    if(2*n+2<v->size()&&comparator(v->at(2*n+2),v->at(c)))
        c = 2*n+2;
    if(2*n+1<v->size()&&comparator(v->at(2*n+1),v->at(c)))
        c = 2*n+1;
    if(c!=n){
        T t = v->at(n);
        v->at(n) = v->at(c);
        v->at(c) = t;
        fix_down(c, comparator);
    }

}

template <typename T>
void binary_heap<T>::insert(T x, bool(*comparator)(T,T)) {
    v->push_back(x);
    fix_up(v->size()-1, comparator);
}

template <typename T>
void binary_heap<T>::make(bool(*comparator)(T,T)) {
    for(int i=v->size()/2-1;i>=0;i--)
        fix_down(i,comparator);
}

template <typename T>
void binary_heap<T>::pop(bool(*comparator)(T,T)){
    T last = v->back();
    v->pop_back();
    if (v->size()!=0){
        v->at(0)=last;
        fix_down(0,comparator);
    }
}

template<typename T>
T binary_heap<T>::top() {
    return v->at(0);
}

template<typename T>
bool binary_heap<T>::empty() {
    return v->empty();
}

template<typename T>
int binary_heap<T>::size() {
    return v->size();
}

template<typename T>
void binary_heap<T>::print(int i, bool left, int ht) {
    if(i>=v->size()){
        return;
    }
    print(2*i+2,(1==0),ht+1);
    for(int i=0;i<ht;i++){
        cout<<'\t';
    }
    if(left){
        cout<<'\\';
    } else {
        cout<<'/';
    }
    cout<<v->at(i)<<endl;
    print(2*i+1,(1==1),ht+1);
}

template<typename T>
void binary_heap<T>::print() {
    print(0,(1==1),0);
}

int main() {
    int a[] = {1,2,3,4,5,6,7,8,9,10,11,12,13};
    int sum=0;
    int k=4;
    binary_heap<int> *heap = new binary_heap<int>();
    for(int i=0;i<13;i++){
        if(heap->size()<k){
            heap->insert(a[i]);
            sum += a[i];
        } else if(heap->top()<a[i]) {
            sum = sum + a[i] - heap->top();
            heap->pop();
            heap->insert(a[i]);
        }
        cout<<i<<" "<<sum<<endl;
    }
}
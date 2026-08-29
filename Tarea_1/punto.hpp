
template <typename T>

class punto
{
private:
    T x;
    T y;
    T z;
public:
    punto();
    punto(T x, T y);
    punto(const punto<T> &p);

    double distancia(const punto<T> &p);

    bool operator==(const punto<T> &p);

    std::ostream &operator<<(std::ostream &os,const punto<T> &p);

    ~punto();
};
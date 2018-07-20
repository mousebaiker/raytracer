#ifndef RAYTRACER_LINAL
#define RAYTRACER_LINAL

#include <math.h>

// Testing macro.
#define EXPECT_VECTORS_NEAR(v1, v2, tol) \
EXPECT_NEAR(v1.x(), v2.x(), tol); \
EXPECT_NEAR(v1.y(), v2.y(), tol); \
EXPECT_NEAR(v1.z(), v2.z(), tol)

namespace linal {

template <class T>
class Vector {
public:
  template <class X, class Y, class Z>
  Vector(X x, Y y, Z z) {
    vec_[0] = x;
    vec_[1] = y;
    vec_[2] = z;
  }

  Vector(const Vector& mE)            = default;
  Vector(Vector&& mE)                 = default;
  Vector& operator=(const Vector& mE) = default;
  Vector& operator=(Vector&& mE)      = default;

  T& x() {return vec_[0];}
  T& y() {return vec_[1];}
  T& z() {return vec_[2];}
  const T& x() const {return vec_[0];}
  const T& y() const {return vec_[1];}
  const T& z() const {return vec_[2];}

  bool operator == (const Vector<T>& other) const {
    return (x() == other.x() && y() == other.y() && z() == other.z());
  }

  bool operator != (const Vector<T>& other) const {
    return !(*this == other);
  }

  Vector<T>& operator += (const Vector<T>& other) {
    x() += other.x();
    y() += other.y();
    z() += other.z();
    return *this;
  }

  Vector<T> operator + (const Vector<T>& other) const {
    Vector<T> temp(*this);
    temp += other;
    return temp;
  }

  Vector<T>& operator -= (const Vector<T>& other) {
    x() -= other.x();
    y() -= other.y();
    z() -= other.z();
    return *this;
  }

  Vector<T> operator - (const Vector<T>& other) const {
    Vector<T> temp(*this);
    temp -= other;
    return temp;
  }

  Vector<T> operator - () const {
    Vector<T> temp(-this->x(), -this->y(), -this->z());
    return temp;
  } 

  template <class U>
  Vector<T>& operator *= (U scalar) {
    x() *= scalar;
    y() *= scalar;
    z() *= scalar;
    return *this;
  }

  template <class U>
  Vector<T> operator * (U scalar) const {
    Vector<T> temp(*this);
    temp *= scalar;
    return temp;
  }

  T dot(const Vector<T>& other) const {
    return x() * other.x() + y() * other.y() + z() * other.z();
  }

  Vector<T> cross(const Vector<T>& other) const {
    T new_x = y() * other.z() - z() * other.y();
    T new_y = z() * other.x() -  x() * other.z();
    T new_z = x() * other.y() - y() * other.x();
    return Vector<T> (new_x, new_y, new_z);
  }

  T length() const {
    return sqrt(x() * x() + y() * y() + z() * z());
  }

  Vector<T> normalized() const {
    T len = length();
    return Vector<T>(x()/len, y()/len, z()/len);
  }

  static Vector<T> zeros() {
    return Vector<T>(0, 0, 0);
  }

private:
  T vec_[3];
};


template <class T, class U>
inline Vector<T> operator * (U scalar, Vector<T> v) {
  return v * scalar;
}

template <class T>
T dot(Vector<T> first, const Vector<T>& second) {
  first.dot(second);
  return first;
}

template <class T>
Vector<T> cross(Vector<T> first, const Vector<T>& second) {
  first.cross(second);
  return first;
}

template <class T, class U>
bool IsClose(const Vector<T>& first, const Vector<T>& second, U tolerance) {
  return (first - second).length() < tolerance;
}

} // namespace linal
#endif //RAYTRACER_LINAL

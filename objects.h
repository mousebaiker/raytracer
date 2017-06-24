#ifndef RAYTRACER_OBJECTS
#define RAYTRACER_OBJECTS

#include "linal.h"

using linal::Vector;

namespace raytracer {
template <class T> 
class Ray {
 public:
  Ray(Vector<T> origin, Vector<T> direction) : origin_(origin), direction_(direction) {}

  Vector<T> GetDirection() const {
    return direction_;
  }

  Vector<T> SetDirection(Vector<T> v) {
    direction_ = v; 
  }

  Vector<T> GetOrigin() const {
    return origin_;
  }

  Vector<T> SetOrigin(Vector<T> v) {
    origin_ = v; 
  }
 
 private:
  Vector<T> direction_;
  Vector<T> origin_;
};


template <class T>
class SceneObject {
 public: 
  SceneObject(Vector<T> position) : position_(position) {}

  Vector<T> GetPosition() const {
   return position_;
  }

  void SetPostion(Vector<T> v) { 
   position_ = v; 
  } 
  
  virtual bool IsIntersected(const Ray<T>& ray) const = 0;
 private:
  Vector<T> position_; 
};


template <class T>
class Sphere : public SceneObject<T> { 
 public: 
  Sphere(Vector<T> center, T radius) : SceneObject<T>(center), radius_(radius) {}
  
  bool IsIntersected(const Ray<T>& ray) const {  
    return IsPointInside(GetProjection(ray));
  }

  bool IsPointInside(const Vector<T>& point) const {
    return RadiusSquaredDifference(point) >= 0;
  }

  Vector<T> GetIntersection(const Ray<T>& ray) const {
    Vector<T> projection = GetProjection(ray);
    T discriminant = RadiusSquaredDifference(projection); 
    
    if (discriminant < 0){
      return Vector<T>::zeros();
    }

    T length = sqrt(discriminant);
    T sign = 1;
    Vector<T> norm_direction = ray.GetDirection().normalized();
    if (!IsPointInside(ray.GetOrigin())) {
      sign *= -1;
    }
    
    Vector<T> intersection_point = projection + sign*length*norm_direction;
    return intersection_point;
  }

 private: 
  Vector<T> GetProjection(const Ray<T>& ray) const {
    const Vector<T> direction = ray.GetDirection().normalized();
    Vector<T> to_center = this->GetPosition() - ray.GetOrigin();
   
    T length = direction.dot(to_center);
    Vector<T> intersection = ray.GetOrigin() + length*ray.GetDirection().normalized();
    return intersection;
  }

  T RadiusSquaredDifference(const Vector<T>& point) const {
    Vector<T> relative_point = point - this->GetPosition();
    return radius_*radius_ - relative_point.dot(relative_point); 
  }

  T radius_;
};
} // namespace raytracer
#endif //RAYTRACER_OBJECTS

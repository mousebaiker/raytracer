#ifndef RAYTRACER_OBJECTS
#define RAYTRACER_OBJECTS

#include "colors.h"
#include "linal.h"

using linal::Vector;

namespace raytracer {

const double TOLERANCE = 1e-7;

template <class T>
class Ray {
 public:
  Ray(Vector<T> origin, Vector<T> direction)
      : origin_(origin), direction_(direction) {}

  Vector<T> GetDirection() const { return direction_; }
  Vector<T> SetDirection(Vector<T> v) { direction_ = v; }

  Vector<T> GetOrigin() const { return origin_; }
  Vector<T> SetOrigin(Vector<T> v) { origin_ = v; }

 private:
  Vector<T> direction_;
  Vector<T> origin_;
};

template <class T>
class SceneObject {
 public:
  SceneObject(Vector<T> position) : position_(position) {}

  Vector<T> GetPosition() const { return position_; }
  void SetPostion(Vector<T> v) { position_ = v; }

  RGB GetColor() const { return color_; }
  void SetColor(RGB color) { color_ = color; }

  double GetAmbient() const { return ambient_; }
  void SetAmbient(double ambient) { ambient_ = ambient; }

  double GetLambert() const { return lambert_; }
  void SetLambert(double lambert) { lambert_ = lambert; }

  double GetSpecular() const { return specular_; }
  void SetSpecular(double specular) { specular_ = specular; }

  virtual bool IsIntersected(const Ray<T> &ray) const = 0;
  virtual Vector<T> GetIntersection(const Ray<T> &ray) const = 0;

  virtual bool Reflect(const Ray<T> &ray, const Vector<T> point) const = 0;

 private:
  Vector<T> position_;
  RGB color_;
  double ambient_;
  double lambert_;
  double specular_;
};

template <class T>
class Sphere : public SceneObject<T> {
 public:
  Sphere(Vector<T> center, T radius)
      : SceneObject<T>(center), radius_(radius) {}

  bool IsIntersected(const Ray<T> &ray) const {
    return IsPointInside(GetProjection(ray));
  }

  bool IsPointInside(const Vector<T> &point) const {
    return RadiusSquaredDifference(point) >= 0;
  }

  Vector<T> GetIntersection(const Ray<T> &ray) const {
    Vector<T> projection = GetProjection(ray);
    T discriminant = RadiusSquaredDifference(projection);

    if (discriminant < 0) {
      return Vector<T>::zeros();
    }

    T length = sqrt(discriminant);
    T sign = 1;
    Vector<T> norm_direction = ray.GetDirection().normalized();
    if (!IsPointInside(ray.GetOrigin())) {
      sign *= -1;
    }

    Vector<T> intersection_point = projection + sign * length * norm_direction;
    return intersection_point;
  }

  Vector<T> GetNormalVector(const Vector<T>& point) const { 
    return point - this->GetPosition();
  }

  Ray<T> Reflect(const Ray<T> &ray, const Vector<T> &point) const {
    // The point is assumed to be on a surface of a sphere.
    Vector<T> to_point = point - ray.GetOrigin();

    if (!linal::IsClose(to_point.normalized(), ray.GetDirection().normalized(), TOLERANCE)) {
      return Ray<T>(Vector<T>::zeros(), Vector<T>::zeros());
    }

    Vector<T> normal = GetNormalVector(point).normalized();
    Vector<T> projection_on_normal = point + to_point.dot(normal) * normal;
    Vector<T> end_point = 2 * (projection_on_normal - ray.GetOrigin()) + ray.GetOrigin();
    return Ray<T>(point, end_point - point);
  }


 private:
  Vector<T> GetProjection(const Ray<T> &ray) const {
    const Vector<T> direction = ray.GetDirection().normalized();
    Vector<T> to_center = this->GetPosition() - ray.GetOrigin();

    T length = direction.dot(to_center);
    Vector<T> intersection =
        ray.GetOrigin() + length * direction;
    return intersection;
  }

  T RadiusSquaredDifference(const Vector<T> &point) const {
    Vector<T> relative_point = point - this->GetPosition();
    return radius_ * radius_ - relative_point.dot(relative_point);
  }

  T radius_;
};

template <class T>
class Scene {
 public:
  void AddCamera(Camera<T> camera) { cameras_.push_back(camera); }

  void PopCamera() {
    if (cameras_.size() > 0) {
      cameras_.pop_back();
    }
  }

  void AddLight(Light<T> light) { lights_.push_back(light); }

  void PopLight() {
    if (lights_.size() > 0) {
      lights_.pop_back();
    }
  }

  void AddObject(SceneObject<T> *object) { objects_.push_back(object); }

  void PopObject() {
    if (objects_.size() > 0) {
      objects_.pop_back();
    }
  }

  void RenderAll() {
    for (auto camera : cameras_) {
      Render(camera);
    }
  }

  void Render(Camera<T> camera) {
    Vector<T> front = camera.GetFront().normalized();
    Vector<T> up = camera.GetUp().normalized();
    Vector<T> right = front.cross(up).normalized();

    long long height = camera.GetHeight();
    long long width = camera.GetWidth();

    double angle_of_view = camera.GetAngleInRads();
    double width_of_view = 2 * tan(angle_of_view);
    double height_of_view = width_of_view * height / (double)width;
    double pixel_size = width_of_view / width;

    Vector<T> width_offset =
        (-1) * (width / 2 - 0.5 * (1 - width % 2)) * pixel_size * right;
    Vector<T> height_offset =
        (height / 2 - 0.5 * (1 - height % 2)) * pixel_size * up;
    for (long long row = 0; row < height; ++row) {
      for (long long column = 0; column < width; ++column) {
        Vector<T> width_direction = width_offset + column * pixel_size * right;
        Vector<T> height_direction =
            height_offset + row * pixel_size * (-1) * up;
        Vector<T> direction = front + width_direction + heigth_direction;
        Ray camera_ray{camera.GetPosition(), direction};
        RGB color_of_pixel =
            GetColor(camera_ray, 0, camera.GetMaxRenderDepth());
        camera.SetPixel(row, column, color_of_pixel);
      }
    }
  }

  RGB GetColor(Ray<T> ray, unsigned int depth, unsigned int max_depth) {
    if (depth > max_depth) {
      return BLACK;
    }

    SceneObject<T> *intersected_object = None;
    Vector<T> closest_intersection;
    T closest_intersection_length = -1;
    for (auto object : objects_) {
      if (object->IsIntersected(ray)) {
        Vector<T> intersection = object->GetIntersection(ray);
        Vector<T> relative_intersection = intersection - ray.GetOrigin();
        if (relative_intersection.length() < closest_point_length) {
          // TODO: Probably would need to avoid intersecting with itself. Try
          // checking for >10e-3 or something similar.
          intersected_object = object;
          closest_intersection = intersection;
          closest_intersection_length = relative_intersection.length();
        }
      }
    }

    // TODO: Can totally make different background color
    if (!intersected_object) {
      return BLACK;
    }

    Ray<T> reflected_ray =
        intersected_object->Reflect(ray, closest_intersection);
    RGB reflection_color = GetColor(reflected_ray, depth + 1, max_depth);
    RGB lightning_color =
        GetLightningColor(closest_intersection, intersected_object);
    RGB base_color = intersected_object->GetColor();
    return base_color * intersected_object->GetAmbient() +
           lightning_color * intersected_object->GetLambert() +
           reflection_color * intersected_object->GetSpecular();
  }

 private:
  std::vector<Camera<T>> cameras_;
  std::vector<Light<T>> lights_;
  std::vector<SceneObject<T> *> objects_;
};

// Scene {
//
// ReadFromXml(path); ??
// Render();
// GetColor(ray, depth);
/
// camera;
// lights;
// objects;
//}

}  // namespace raytracer
#endif  // RAYTRACER_OBJECTS

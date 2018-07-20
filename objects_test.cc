#include <cmath>

#include "linal.h"
#include "objects.h"
#include "gtest/gtest.h"

namespace {

using linal::Vector;
using raytracer::Sphere;
using raytracer::Ray;
using Vector_f = Vector<float>;
using Sphere_f = Sphere<float>;
using Ray_f = Ray<float>;


TEST(SphereTest, RayAlongAxisIntersectionTest) {
  Vector_f center(0, 0, 0);
  float radius = 2;
  Sphere_f sphere(center, radius);

  Ray_f horisontal_ray(Vector_f(-5, 0, 0), Vector_f(1, 0 ,0));
  EXPECT_TRUE(sphere.IsIntersected(horisontal_ray));
  Vector_f intersection = sphere.GetIntersection(horisontal_ray);
  EXPECT_VECTORS_NEAR(intersection, Vector_f(-2, 0, 0), raytracer::kTolerance);
}

TEST(SphereTest, GeneralCaseIntersectionTest) {
  Vector_f center(1, 1, 1);
  float radius = 2;
  Sphere_f sphere(center, radius);

  Ray_f ray(Vector_f(0, -3., 0), Vector_f(0.5, 1, 0.5));
  EXPECT_TRUE(sphere.IsIntersected(ray));
  Vector_f intersection = sphere.GetIntersection(ray);
  float offset = 2;
  Vector_f correct_intersection = ray.GetOrigin() + offset * ray.GetDirection();
  EXPECT_VECTORS_NEAR(intersection, correct_intersection, raytracer::kTolerance);
}

TEST(SphereTest, RayInsideIntersectionTest) {
  Vector_f center(0, 0, 0);
  float radius = 2;
  Sphere_f sphere(center, radius);

  Ray_f internal_ray(Vector_f::zeros(), Vector_f(1, 1, 1));
  EXPECT_TRUE(sphere.IsIntersected(internal_ray));
  Vector_f intersection = sphere.GetIntersection(internal_ray);
  float offset = 2./3 * std::sqrt(3);
  Vector_f correct_intersection = internal_ray.GetOrigin() + offset * internal_ray.GetDirection();
  EXPECT_VECTORS_NEAR(intersection, correct_intersection, raytracer::kTolerance);
}

TEST(SphereTest, NoIntersectionTest) {
  Vector_f center(0, 0, 0);
  float radius = 2;
  Sphere_f sphere(center, radius);

  Ray_f internal_ray(Vector_f(0, -3, 0), Vector_f(1, 1, 1));
  EXPECT_FALSE(sphere.IsIntersected(internal_ray));
  Vector_f intersection = sphere.GetIntersection(internal_ray);
  Vector_f correct_intersection = Vector_f::zeros();
  EXPECT_VECTORS_NEAR(intersection, correct_intersection, raytracer::kTolerance);
}

TEST(SphereTest, IsPointInsideTest) {
  Vector_f center(1, -1, 1);
  float radius = 2;
  Sphere_f sphere(center, radius);

  EXPECT_TRUE(sphere.IsPointInside(Vector_f::zeros()));
  EXPECT_TRUE(sphere.IsPointInside(Vector_f(0.4, 0.6, 0.1)));
}


TEST(SphereTest, GetNormalTest) {
  Vector_f center(1, 1, 1);
  float radius = 1;
  Sphere_f sphere(center, radius);

  auto normal = sphere.GetNormalVector(Vector_f(1, 2, 1));
  Vector_f correct_normal(0, 1, 0);
  EXPECT_VECTORS_NEAR(normal, correct_normal, raytracer::kTolerance);

  Vector_f point(std::sqrt(3)/3, std::sqrt(3)/3, std::sqrt(3)/3);
  normal = sphere.GetNormalVector(point + center);
  correct_normal = point;
  EXPECT_VECTORS_NEAR(normal, correct_normal, raytracer::kTolerance);
}

TEST(SphereTest, ReflectStraightDownRayTest) {
  Vector_f center(1, 1, 1);
  float radius = 1;
  Sphere_f sphere(center, radius);

  Ray_f ray(Vector_f(1, 3, 1), Vector_f(0, -0.5, 0));
  auto intersection =  sphere.GetIntersection(ray);
  Vector_f correct_intersection(1, 2, 1);
  auto reflected_ray = sphere.Reflect(ray, intersection);
  Ray_f correct_reflected_ray(correct_intersection, Vector_f(0, 1, 0));
  EXPECT_VECTORS_NEAR(intersection, correct_intersection, raytracer::kTolerance);

  EXPECT_VECTORS_NEAR(reflected_ray.GetOrigin(), correct_reflected_ray.GetOrigin(), raytracer::kTolerance);

  auto direction = reflected_ray.GetDirection().normalized();
  auto correct_direction = correct_reflected_ray.GetDirection().normalized();
  EXPECT_VECTORS_NEAR(direction, correct_direction, raytracer::kTolerance);
}


TEST(SphereTest, TangentRayReflectTest) {
  Vector_f center(1, 1, 1);
  float radius = 1;
  Sphere_f sphere(center, radius);

  Ray_f ray(Vector_f(1, 0, 0), Vector_f(0, 0, 0.5));
  auto intersection =  sphere.GetIntersection(ray);
  Vector_f correct_intersection(1, 0, 1);
  auto reflected_ray = sphere.Reflect(ray, intersection);
  Ray_f correct_reflected_ray(correct_intersection, Vector_f(0, 0, 1));
  EXPECT_VECTORS_NEAR(intersection, correct_intersection, raytracer::kTolerance);

  EXPECT_VECTORS_NEAR(reflected_ray.GetOrigin(), correct_reflected_ray.GetOrigin(), raytracer::kTolerance);

  auto direction = reflected_ray.GetDirection().normalized();
  auto correct_direction = correct_reflected_ray.GetDirection().normalized();
  EXPECT_VECTORS_NEAR(direction, correct_direction, raytracer::kTolerance);
}

TEST(SphereTest, InsideReflectionTest) {
  Vector_f center(1, 1, 1);
  float radius = 1;
  Sphere_f sphere(center, radius);

  Vector_f correct_intersection = center + Vector_f(std::sqrt(3), std::sqrt(3), 1).normalized();
  Vector_f origin(1, 1.5, 1);
  Vector_f direction = correct_intersection - origin;
  direction *= 0.5;

  Ray_f ray(origin, direction);
  auto intersection =  sphere.GetIntersection(ray);
  auto reflected_ray = sphere.Reflect(ray, intersection);

  Vector_f correct_direction = Vector_f(1.5, 1, 1) - correct_intersection;
  Ray_f correct_reflected_ray(correct_intersection, correct_direction);
  EXPECT_VECTORS_NEAR(intersection, correct_intersection, raytracer::kTolerance);

  EXPECT_VECTORS_NEAR(reflected_ray.GetOrigin(), correct_reflected_ray.GetOrigin(), raytracer::kTolerance);

  auto reflected_direction = reflected_ray.GetDirection().normalized();
  auto correct_reflected_direction = correct_reflected_ray.GetDirection().normalized();
  EXPECT_VECTORS_NEAR(reflected_direction, correct_reflected_direction, raytracer::kTolerance);
}
} // namespace

#include "polygon.hpp"
#include <iostream>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <delimiter.hpp>
#include "predicates.hpp"
#include "functors.hpp"

std::istream & erohin::operator>>(std::istream & input, Point & point)
{
  std::istream::sentry sentry(input);
  if (!sentry)
  {
    return input;
  }
  Point temp;
  using del = IgnoredCaseDelimiter;
  input >> del{'('} >> point.x >> del{';'} >> point.y >> del{')'};
  if (!input)
  {
    point = temp;
  }
  return input;
}

std::ostream & erohin::operator<<(std::ostream & output, const Point & point)
{
  std::ostream::sentry sentry(output);
  if (!sentry)
  {
    return output;
  }
  output << '(' << point.x << ';' << point.y << ')';
  return output;
}

std::istream & erohin::operator>>(std::istream & input, Polygon & polygon)
{
  std::istream::sentry sentry(input);
  if (!sentry)
  {
    return input;
  }
  size_t count = 0;
  input >> count;
  if (count < 3)
  {
    input.setstate(std::ios::failbit);
    return input;
  }
  std::vector< Point > temp;
  temp.reserve(count);
  std::copy_n(
    std::istream_iterator< Point >(input),
    count,
    std::back_inserter(temp)
  );
  if (input)
  {
    polygon.points = std::move(temp);
  }
  return input;
}

std::ostream & erohin::operator<<(std::ostream & output, const Polygon & polygon)
{
  std::ostream::sentry sentry(output);
  if (!sentry)
  {
    return output;
  }
  output << polygon.points.size() << " ";
  std::copy(
    polygon.points.cbegin(),
    polygon.points.cend(),
    std::ostream_iterator< Point >(output, " ")
  );
  return output;
}

double erohin::getArea(const Polygon & polygon)
{
  const std::vector< Point > & vertex = polygon.points;
  auto area_functor = evaluateArea{ *vertex.cbegin(), *vertex.cbegin() };
  std::vector< double > part_area;
  std::transform(vertex.cbegin(), vertex.cend(), std::back_inserter(part_area), area_functor);
  return std::accumulate(part_area.cbegin(), part_area.cend(), 0);
}
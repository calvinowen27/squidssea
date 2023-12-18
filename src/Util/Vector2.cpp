#include "../../include/game/Util/Vector2.hpp"

#include <cmath>
#include <sstream>

/* Vector2 */
Vector2 Vector2::zero;

Vector2::Vector2()
{
    this->x = 0;
    this->y = 0;
}
Vector2::Vector2(float x, float y)
{
    this->x = x;
    this->y = y;
}

Vector2::Vector2(const Vector2 &b)
{
    this->x = b.x;
    this->y = b.y;
}

float Vector2::distance(const Vector2 &a, const Vector2 &b)
{
    return (float)(sqrt(pow(b.x-a.x, 2)+pow(b.y-a.y, 2)));
}

bool Vector2::equals(const Vector2& b) const
{
    return x == b.x && y == b.y;
}

void Vector2::normalize()
{
    if(x != 0 || y != 0) *this /= sqrt(x*x + y*y);
}

Vector2 Vector2::normalized() const
{
    Vector2 result = *this;
    result.normalize();
    return result;
}

float Vector2::magnitude() const
{
    return (float)sqrt(x*x+y*y);
}

Vector2 Vector2::getSigns() const
{
    Vector2 result(x == 0 ? 0 : x / fabs(x), y == 0 ? 0 : y / fabs(y));
    return result;
}

Vector2 Vector2::abs() const
{
    Vector2 result(fabs(x), fabs(y));
    return result;
}

Vector2 Vector2::round(int decimals) const
{
    int s = pow(10, decimals);
    Vector2 result(roundf(x*s)/s, roundf(y*s)/s);
    return result;
}

bool operator==(const Vector2& a, const Vector2& b)
{
    return a.equals(b);
}

bool operator!=(const Vector2& a, const Vector2& b)
{
    return !a.equals(b);
}

bool operator<(const Vector2& a, const Vector2& b)
{
    return a.magnitude() < b.magnitude();
}

Vector2 Vector2::operator+(const Vector2 &b) const
{
    return Vector2(x+b.x, y+b.y);
}

Vector2 Vector2::operator-(const Vector2 &b) const
{
    return Vector2(x-b.x, y-b.y);
}

Vector2 Vector2::operator*(const float &a) const
{
    return Vector2(x*a, y*a);
}

Vector2 Vector2::operator*(const Vector2& b) const
{
    return Vector2(x*b.x, y*b.y);
}

Vector2 Vector2::operator/(const float &a) const
{
    return Vector2(x/a, y/a);
}

Vector2 Vector2::operator/(const Vector2& b) const
{
    return Vector2(x/b.x, y/b.y);
}

Vector2& Vector2::operator=(const Vector2 &b)
{
    x = b.x;
    y = b.y;

    return *this;
}

Vector2& Vector2::operator+=(const Vector2 &b)
{
    x += b.x;
    y += b.y;

    return *this;
}

Vector2& Vector2::operator-=(const Vector2 &b)
{
    x -= b.x;
    y -= b.y;

    return *this;
}

Vector2& Vector2::operator*=(const float &a)
{
    x *= a;
    y *= a;

    return *this;
}

Vector2& Vector2::operator/=(const float &a)
{
    x /= a;
    y /= a;

    return *this;
}

std::string Vector2::to_string() const
{
    std::stringstream ss;
    ss << "(" << x << ", " << y << ")";
    return ss.str();
}

std::ostream& operator<<(std::ostream& stream, const Vector2& v)
{
    stream << v.Vector2::to_string();
    return stream;
}

Vector2::operator Vector2Int() const
{
    return Vector2Int((int)x, (int)y);
}

/* Vector2Int */

Vector2Int::Vector2Int()
{
    this->x = 0;
    this->y = 0;
}

Vector2Int::Vector2Int(int x, int y)
{
    this->x = x;
    this->y = y;
}

Vector2Int::Vector2Int(float x, float y)
{
    this->x = (int)x;
    this->y = (int)y;
}

Vector2Int::Vector2Int(double x, double y)
{
    this->x = (int)x;
    this->y = (int)y;
}

Vector2Int::Vector2Int(const Vector2Int &b)
{
    this->x = b.x;
    this->y = b.y;
}

float Vector2Int::distance(const Vector2Int &a, const Vector2Int &b)
{
    return (float)(sqrt(pow(b.x-a.x, 2)+pow(b.y-a.y, 2)));
}

bool Vector2Int::equals(const Vector2Int &b) const
{
    return x == b.x && y == b.y;
}

void Vector2Int::normalize()
{
    if(x != 0 && y != 0) *this /= (sqrt(pow(x, 2) + pow(y, 2)));
}

Vector2Int Vector2Int::normalized() const
{
    Vector2Int result = *this;
    result.normalize();
    return result;
}

bool operator==(const Vector2Int &a, const Vector2Int &b)
{
    return a.equals(b);
}

bool operator!=(const Vector2Int &a, const Vector2Int &b)
{
    return !a.equals(b);
}

Vector2Int Vector2Int::operator+(const Vector2Int &b) const
{
    return Vector2Int(x+b.x, y+b.y);
}

Vector2Int Vector2Int::operator-(const Vector2Int &b) const
{
    return Vector2Int(x-b.x, y-b.y);
}

Vector2Int Vector2Int::operator*(const float &a) const
{
    return Vector2Int((int)(x*a), (int)(y*a));
}

Vector2Int Vector2Int::operator*(const Vector2Int &b) const
{
    return Vector2Int((int)(x*b.x), (int)(y*b.y));
}

Vector2Int Vector2Int::operator/(const float &a) const
{
    return Vector2Int((int)(x/a), (int)(y/a));
}

Vector2Int& Vector2Int::operator=(const Vector2Int &b)
{
    x = b.x;
    y = b.y;

    return *this;
}

Vector2Int& Vector2Int::operator+=(const Vector2Int &b)
{
    x += b.x;
    y += b.y;

    return *this;
}

Vector2Int& Vector2Int::operator-=(const Vector2Int &b)
{
    x -= b.x;
    y -= b.y;

    return *this;
}

Vector2Int& Vector2Int::operator*=(const float &a)
{
    x = (int)(x*a);
    y = (int)(y*a);

    return *this;
}

Vector2Int& Vector2Int::operator/=(const float &a)
{
    x = (int)(x/a);
    y = (int)(y/a);

    return *this;
}

std::string Vector2Int::to_string() const
{
    std::stringstream ss;
    ss << "(" << x << ", " << y << ")";
    return ss.str();
}

std::ostream& operator<<(std::ostream& stream, const Vector2Int& v)
{
    stream << v.Vector2Int::to_string();
    return stream;
}

Vector2Int::operator Vector2() const
{
    return Vector2(x, y);
}
#pragma once

class AABB
{
public:
    bool exist = false;

    float epsilon = 0.0F;
    double x0;
    double y0;
    double z0;
    double x1;
    double y1;
    double z1;

    double getMinX()
    {
        return this->x0;
    }

    double getMinY()
    {
        return this->y0;
    }

    double getMinZ()
    {
        return this->z0;
    }

    double getMaxX()
    {
        return this->x1;
    }

    double getMaxY()
    {
        return this->y1;
    }

    double getMaxZ()
    {
        return this->z1;
    }

    bool getExist()
    {
        return exist;
    }

    void setExist(bool e)
    {
        exist = e;
    }

    float clipXCollide(AABB c, float xa) {
        if (!(c.y1 <= this->y0) && !(c.y0 >= this->y1)) {
            if (!(c.z1 <= this->z0) && !(c.z0 >= this->z1)) {
                float max;
                if (xa > 0.0F && c.x1 <= this->x0) {
                    max = this->x0 - c.x1 - this->epsilon;
                    if (max < xa) {
                        xa = max;
                    }
                }

                if (xa < 0.0F && c.x0 >= this->x1) {
                    max = this->x1 - c.x0 + this->epsilon;
                    if (max > xa) {
                        xa = max;
                    }
                }

                return xa;
            } else {
                return xa;
            }
        } else {
            return xa;
        }
    }

    float clipYCollide(AABB c, float ya) {
        if (!(c.x1 <= this->x0) && !(c.x0 >= this->x1)) {
            if (!(c.z1 <= this->z0) && !(c.z0 >= this->z1)) {
                float max;
                if (ya > 0.0F && c.y1 <= this->y0) {
                    max = this->y0 - c.y1 - this->epsilon;
                    if (max < ya) {
                        ya = max;
                    }
                }

                if (ya < 0.0F && c.y0 >= this->y1) {
                    max = this->y1 - c.y0 + this->epsilon;
                    if (max > ya) {
                        ya = max;
                    }
                }

                return ya;
            } else {
                return ya;
            }
        } else {
            return ya;
        }
    }

    float clipZCollide(AABB c, float za) {
        if (!(c.x1 <= this->x0) && !(c.x0 >= this->x1)) {
            if (!(c.y1 <= this->y0) && !(c.y0 >= this->y1)) {
                float max;
                if (za > 0.0F && c.z1 <= this->z0) {
                    max = this->z0 - c.z1 - this->epsilon;
                    if (max < za) {
                        za = max;
                    }
                }

                if (za < 0.0F && c.z0 >= this->z1) {
                    max = this->z1 - c.z0 + this->epsilon;
                    if (max > za) {
                        za = max;
                    }
                }

                return za;
            } else {
                return za;
            }
        } else {
            return za;
        }
    }

    void move(float xa, float ya, float za) {
        this->x0 += xa;
        this->y0 += ya;
        this->z0 += za;
        this->x1 += xa;
        this->y1 += ya;
        this->z1 += za;
    }

    AABB(double minx, double miny, double minz, double maxx, double maxy, double maxz)
    {
        this->x0 = minx;
        this->y0 = miny;
        this->z0 = minz;
        this->x1 = maxx;
        this->y1 = maxy;
        this->z1 = maxz;
    }

    AABB expand(float xa, float ya, float za) {
        float _x0 = this->x0;
        float _y0 = this->y0;
        float _z0 = this->z0;
        float _x1 = this->x1;
        float _y1 = this->y1;
        float _z1 = this->z1;
        if (xa < 0.0F) {
            _x0 += xa;
        }

        if (xa > 0.0F) {
            _x1 += xa;
        }

        if (ya < 0.0F) {
            _y0 += ya;
        }

        if (ya > 0.0F) {
            _y1 += ya;
        }

        if (za < 0.0F) {
            _z0 += za;
        }

        if (za > 0.0F) {
            _z1 += za;
        }

        return AABB(_x0, _y0, _z0, _x1, _y1, _z1);
    }

};

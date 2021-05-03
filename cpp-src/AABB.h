#pragma once

class AABB
{
public:
    bool exist = false;

    float epsilon = 0.0F;
    double minX;
    double minY;
    double minZ;
    double maxX;
    double maxY;
    double maxZ;

    double getMinX()
    {
        return this->minX;
    }

    double getMinY()
    {
        return this->minY;
    }

    double getMinZ()
    {
        return this->minZ;
    }

    double getMaxX()
    {
        return this->maxX;
    }

    double getMaxY()
    {
        return this->maxY;
    }

    double getMaxZ()
    {
        return this->maxZ;
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
        if (!(c.maxY <= this->minY) && !(c.minY >= this->maxY)) {
            if (!(c.maxZ <= this->minZ) && !(c.minZ >= this->maxZ)) {
                float max;
                if (xa > 0.0F && c.maxX <= this->minX) {
                    max = this->minX - c.maxX - this->epsilon;
                    if (max < xa) {
                        xa = max;
                    }
                }

                if (xa < 0.0F && c.minX >= this->maxX) {
                    max = this->maxX - c.minX + this->epsilon;
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
        if (!(c.maxX <= this->minX) && !(c.minX >= this->maxX)) {
            if (!(c.maxZ <= this->minZ) && !(c.minZ >= this->maxZ)) {
                float max;
                if (ya > 0.0F && c.maxY <= this->minY) {
                    max = this->minY - c.maxY - this->epsilon;
                    if (max < ya) {
                        ya = max;
                    }
                }

                if (ya < 0.0F && c.minY >= this->maxY) {
                    max = this->maxY - c.minY + this->epsilon;
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
        if (!(c.maxX <= this->minX) && !(c.minX >= this->maxX)) {
            if (!(c.maxY <= this->minY) && !(c.minY >= this->maxY)) {
                float max;
                if (za > 0.0F && c.maxZ <= this->minZ) {
                    max = this->minZ - c.maxZ - this->epsilon;
                    if (max < za) {
                        za = max;
                    }
                }

                if (za < 0.0F && c.minZ >= this->maxZ) {
                    max = this->maxZ - c.minZ + this->epsilon;
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
        this->minX += xa;
        this->minY += ya;
        this->minZ += za;
        this->maxX += xa;
        this->maxY += ya;
        this->maxZ += za;
    }

    AABB(double minx, double miny, double minz, double maxx, double maxy, double maxz)
    {
        this->minX = minx;
        this->minY = miny;
        this->minZ = minz;
        this->maxX = maxx;
        this->maxY = maxy;
        this->maxZ = maxz;
    }

};

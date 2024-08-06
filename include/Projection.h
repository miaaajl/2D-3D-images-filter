#ifndef PROJECTION_H
#define PROJECTION_H

#include "Volume.h"
#include "Image.h"
#include <string>
#include <functional>

/**
 * The Projection class serves as a base class for various projection operations on a Volume.
 * It defines common attributes and methods required for performing projections, such as setting the projection plane
 * and specifying the range of slices to project. Derived classes should implement the executeProjection method
 * to perform the specific projection operation.
 */
class Projection {
public:
    enum Method {
        minimum,
        maximum,
        mean,
        median
    };

    explicit Projection(Method method, int min_depth = -1, int max_depth = -1);

    ~Projection();

    /**
     * Assign the class member function here using lambda fn
     */
    void get_callable();

    /**
     * TODO:-> @Chao
     * @param vol
     * @return
     */
    Image maximum_projection(Volume &vol);

    /**
     * TODO:-> @Chao
     * @param vol
     * @return
     */
    Image minimum_projection(Volume &vol);

    Image mean_projection(Volume &vol);

    Image median_projection(Volume &vol);

    Image execute(Volume& volume);

private:

    Method method{maximum};

    int min_depth{0}, max_depth{0};

    std::function<Image(Volume&)> execution;

};


#endif //PROJECTION_H
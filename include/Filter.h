#ifndef FILTER_H
#define FILTER_H

#include "Image.h"
#include "Volume.h"

/**
 * The Filter class is a template base class designed to be inherited by specific filter implementations.
 * It provides a common interface for applying filters to objects of type T, which is typically an Image
 * or Volume for image and volume processing tasks, respectively.
 *
 * @tparam T The type of object the filter will be applied to, e.g., Image.
 */
template<typename T>
class Filter {

public:
    /**
     * Default constructor for the Filter class.
     * Initializes a new Filter object. This constructor is provided to ensure derived classes
     * can perform their own initialization as needed.
     */
    Filter();

    /**
     * Virtual destructor for the Filter class.
     * Ensures derived classes can clean up resources appropriately. Being virtual guarantees
     * that the destructor of the derived classes is called when deleting an instance through a base class pointer.
     */
    virtual ~Filter();

    /**
     * Pure virtual function that applies the filter to an object of type T.
     * Derived classes must implement this function to perform their specific filtering operation.
     *
     * @param item A reference to the object the filter will be applied to.
     */
    virtual void apply(T &item) = 0;

};



#endif //FILTER_H

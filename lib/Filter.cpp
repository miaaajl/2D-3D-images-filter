#include "Filter.h"


template<typename T>
Filter<T>::Filter() = default;

template<typename T>
Filter<T>::~Filter() = default;

template class Filter<Image>;
template class Filter<Volume>;


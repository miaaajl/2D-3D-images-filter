#include "Volume.h"

Volume::Volume() = default;


Volume::Volume(
    std::string path,
    std::string output_path,
    int min_depth,
    int max_depth,
    int batch_size,
    const std::string& output_format,
    bool load_full_volume
) :
    path(std::move(path)),
    output_path(std::move(output_path)),
    batch_size(batch_size),
    load_full_volume(load_full_volume) {
    load_paths();
    if (load_full_volume) load_data(min_depth, max_depth);
    else load_cache(0);

    if (output_format == "png") {
        write_function = [this](const std::string &filename, int w, int h, unsigned char *buffer) {
            stbi_write_png((filename + ".png").c_str(), w, h, 1, buffer, 0);
        };
    }
    else if (output_format == "bmp") {
        write_function = [this](const std::string &filename, int w, int h, unsigned char *buffer) {
            stbi_write_bmp((filename + ".bmp").c_str(), w, h, 1, buffer);
        };
    }
}

Volume::~Volume() = default;


void Volume::load_paths() {
    for (const auto& file_path: std::filesystem::directory_iterator(path))
        filepaths.push_back(file_path.path().string());

    n = filepaths.size();
    slice_idx = 0;
    quick_sort(filepaths, 0, n - 1);
}


void Volume::load_data(int start, int end) {
    start == -1 ? start = 0 : start;
    end == -1 ? end = n : end;
    n = end - start;
    data.reserve(n);
    for (int i = start; i < end; ++i)
        data.emplace_back(*load_slice(filepaths[i]));
}
void Volume::setN(int depth){
    n = depth;
}


void Volume::load_cache(int start) {
    int range = std::min(batch_size, n - slice_idx);
    for (int i = 0; i < range; ++i) {
        cache.emplace_back(load_slice(filepaths[start + i]));
        slice_idx++;
    }
}


void Volume::shift() {
    if (!load_full_volume) {
        cache.clear();
        load_cache(slice_idx);
    }
}


void Volume::write_batch(int start, int end) {
    std::vector<Matrix<uint8_t>*> b = get_batch(start, end);
    for (int i = start; i < end; ++i) {
        auto& slice = b[i - start];
        write_slice(*slice, i+1);
    }
}


void Volume::write_slice(Matrix<uint8_t> slice, int idx) {
    auto [h, w] = slice.shape();
    unsigned char* buffer = retrieve_buffer(slice, h, w);
    write_function((output_path + "_" + std::to_string(idx)), w, h, buffer);
}


unsigned char* Volume::retrieve_buffer(Matrix<uint8_t> &slice, int h, int w) {
    auto* buffer = new unsigned char[h * w];
    int idx;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            idx = (i * w + j);
            buffer[idx] = slice(i, j);
        }
    }
    return buffer;
}


Matrix<uint8_t>* Volume::load_slice(const std::string& slice_path) {
    int h, w, c;
    unsigned char* buffer = stbi_load(slice_path.c_str(), &w, &h, &c, 1);
    if (buffer == nullptr)
        throw std::runtime_error("Failed to load image at path: " + slice_path);

    auto* slice = new Matrix<uint8_t>(h, w);
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            (*slice)(i, j) = buffer[i * w + j * c];
        }
    }
    delete[] buffer;
    return slice;
}


std::tuple<int, int, int> Volume::shape() {
    auto [h, w] = (load_full_volume) ? data[0].shape() : (*cache[0]).shape();
    return std::make_tuple(h, w, n);
}


std::vector<Matrix<uint8_t>*> Volume::get_batch(int start, int end) {
    std::vector<Matrix<uint8_t>*> b;
    // for preloaded data
    if (load_full_volume) {
        for (int i = start; i < end; ++i)
            b.push_back(&data[i]);
        return b;
    }
    return cache;
}


std::string Volume::summary() {
    auto [h, w, d] = this->shape();
    return "Height = " + std::to_string(h) + ", width = " + std::to_string(w) + ", depth = " + std::to_string(d);
}

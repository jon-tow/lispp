#ifndef FRAME_H
#define FRAME_H

#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

#include "exception.h"
#include "operators.h"
#include "type.h"

class Frame {
      public:
        Frame() = default;
        Frame(std::shared_ptr<Frame> parent) : parent(std::move(parent)) {}

        type::LisppObject lookup(const std::string& sym) const;
        void set(const std::string& sym, const type::LisppObject& value);
        void print_symbols() const;

        static Frame global();

      private:
        std::optional<Frame> find(const std::string& sym) const;

        std::shared_ptr<Frame> parent;
        std::unordered_map<std::string, type::LisppObject> symbols;
};

#endif // FRAME_H

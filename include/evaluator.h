#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "exception.h"
#include "frame.h"
#include "syntax.h"
#include "type.h"
#include <iostream>

namespace evaluator {

type::LisppObject eval(const type::LisppObject& ast, Frame& frame);
type::LisppObject apply(const type::LisppObject& procedure,
                        const std::vector<type::LisppObject>& arguments);

} // namespace evaluator

#endif // EVALUATOR_H

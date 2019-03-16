#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "environment.h"
#include "exception.h"
#include "syntax.h"
#include "types.h"

namespace evaluator {

type::LisppObject eval(const type::LisppObject& ast, Environment& env);
type::LisppObject apply(const type::LisppObject& procedure,
                        const std::vector<type::LisppObject>& arguments);

} // namespace evaluator

#endif

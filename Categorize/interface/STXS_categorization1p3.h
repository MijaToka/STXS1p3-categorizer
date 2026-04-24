#ifndef STXS_CATEGORIZATION_1P3_H
#define STXS_CATEGORIZATION_1P3_H

#include "STXSCategorizer/CommonUtils/interface/STXS_common.h"
#include <ROOT/RDF/InterfaceUtils.hxx>
#include <map>

namespace Version1_3 {
std::map<STXS1, ROOT::RDF::RNode>
second_categorization(std::map<STXS0, ROOT::RDF::RNode> arr);
}

#endif

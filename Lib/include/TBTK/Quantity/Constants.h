/* Copyright 2019 Kristofer Björnson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/// @cond TBTK_FULL_DOCUMENTATION
/** @package TBTKcalc
 *  @file Constants.h
 *  @brief Numerical constants.
 *
 *  @author Kristofer Björnson
 */

#ifndef COM_DAFER45_TBTK_QUANTITY_CONSTANTS
#define COM_DAFER45_TBTK_QUANTITY_CONSTANTS

#include "TBTK/Quantity/Base.h"
#include "TBTK/Quantity/Constant.h"
#include "TBTK/Quantity/Derived.h"
#include "TBTK/TBTK.h"
#include "TBTK/UnitHandler.h"

#include <cmath>
#include <map>
#include <string>

namespace TBTK{
namespace Quantity{

/** @brief Numerical constants.
 *
 *  <b>Warning:</b> All numerical constants defined here are given in the
 *  default base units C, pcs, eV, m, K, s. These are used to initialize the
 *  UnitHandler and Quantities and should not be used directly outside of this
 *  scope. Therefore, request constants through the UnitHandler for all other
 *  purposes. */
class Constants{
private:
	static std::map<std::string, Constant> constants;

	static void initialize();
	static double getValue(const std::string &symbol);
	static std::vector<int> getExponents();

	friend void TBTK::Initialize();
	friend void TBTK::Quantity::initializeBaseQuantities();
	friend void TBTK::Quantity::initializeDerivedQuantities();
	friend class UnitHandler;
};

}; //End of namesapce Quantity
}; //End of namesapce TBTK

#endif
/// @endcond

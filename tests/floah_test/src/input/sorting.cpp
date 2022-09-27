#include "floah_test/input/sorting.h"

////////////////////////////////////////////////////////////////
// Standard includes.
////////////////////////////////////////////////////////////////

#include <algorithm>
#include <array>

////////////////////////////////////////////////////////////////
// Module includes.
////////////////////////////////////////////////////////////////

#include "floah-put/input_element.h"

namespace
{
    class TestElement : public floah::InputElement
    {
    public:
        [[nodiscard]] const InputElement* getInputParent() const noexcept override { return parent; }

        [[nodiscard]] int32_t getInputLayer() const noexcept override { return layer; }

        [[nodiscard]] bool intersect(math::int2) const noexcept override { return true; }

        InputElement* parent = nullptr;
        int32_t       layer  = 0;
        std::string   name;
    };
}  // namespace

namespace test
{
    void InputElementSorting::operator()()
    {
        // Create a whole bunch of elements.
        TestElement elemA, elemB, elemC, elemD, elemE, elemF, elemG, elemH, elemI, elemJ, elemK;
        elemA.layer  = 1;
        elemA.name   = "A";
        elemB.parent = &elemA;
        elemB.layer  = -1;
        elemB.name   = "B";
        elemC.parent = &elemA;
        elemC.layer  = 2;
        elemC.name   = "C";
        elemD.parent = &elemC;
        elemD.layer  = 3;
        elemD.name   = "D";
        elemE.parent = &elemD;
        elemE.layer  = -4;
        elemE.name   = "E";
        elemF.layer  = 2;
        elemF.name   = "F";
        elemG.parent = &elemF;
        elemG.layer  = 1;
        elemG.name   = "G";
        elemH.parent = &elemG;
        elemH.layer  = 5;
        elemH.name   = "H";
        elemI.parent = &elemF;
        elemI.layer  = 3;
        elemI.name   = "I";
        elemJ.parent = &elemI;
        elemJ.layer  = 10;
        elemJ.name   = "J";
        elemK.parent = &elemF;
        elemK.layer  = -5;
        elemK.name   = "K";

        // Sort them.
        std::array elements = {&elemA, &elemB, &elemC, &elemD, &elemE, &elemF, &elemG, &elemH, &elemI, &elemJ, &elemK};
        std::ranges::sort(
          elements, [](const floah::InputElement* lhs, const floah::InputElement* rhs) { return lhs->compare(*rhs); });

        // Verify order.
        compareEQ(elemJ.name, elements[0]->name);
        compareEQ(elemI.name, elements[1]->name);
        compareEQ(elemH.name, elements[2]->name);
        compareEQ(elemG.name, elements[3]->name);
        compareEQ(elemK.name, elements[4]->name);
        compareEQ(elemF.name, elements[5]->name);
        compareEQ(elemE.name, elements[6]->name);
        compareEQ(elemD.name, elements[7]->name);
        compareEQ(elemC.name, elements[8]->name);
        compareEQ(elemB.name, elements[9]->name);
        compareEQ(elemA.name, elements[10]->name);
    }
}  // namespace test

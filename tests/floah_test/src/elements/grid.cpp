#include "floah_test/elements/grid.h"

#include "floah-layout/elements/grid.h"

namespace test
{
    // TODO: None of these tests take place inside of a layout. Adding/removing/extracting children is therefore not tested fully.

    void GridProperties::operator()()
    {
        floah::Grid grid;

        // Check initial state.
        compareEQ(grid.getHorizontalAlignment(), floah::HorizontalAlignment::Left);
        compareEQ(grid.getVerticalAlignment(), floah::VerticalAlignment::Top);
        compareEQ(grid.getRowCount(), 0);
        compareEQ(grid.getColumnCount(), 0);

        // Modify.
        grid.setHorizontalAlignment(floah::HorizontalAlignment::Right);
        grid.setVerticalAlignment(floah::VerticalAlignment::Middle);

        // Check again.
        compareEQ(grid.getHorizontalAlignment(), floah::HorizontalAlignment::Right);
        compareEQ(grid.getVerticalAlignment(), floah::VerticalAlignment::Middle);

        // Copy.
        floah::Grid grid2(grid);
        floah::Grid grid3;
        grid3 = grid;
        compareEQ(grid2.getHorizontalAlignment(), floah::HorizontalAlignment::Right);
        compareEQ(grid2.getVerticalAlignment(), floah::VerticalAlignment::Middle);
        compareEQ(grid3.getHorizontalAlignment(), floah::HorizontalAlignment::Right);
        compareEQ(grid3.getVerticalAlignment(), floah::VerticalAlignment::Middle);
    }

    void GridElements::operator()()
    {
        floah::Grid grid;

        // Add some elements.
        grid.appendRow();
        grid.appendRow();
        grid.appendColumn();
        grid.appendColumn();
        auto& elem0 = grid.insert(std::make_unique<floah::LayoutElement>(), 0, 0);
        auto& elem1 = grid.insert(std::make_unique<floah::LayoutElement>(), 1, 1);
        expectThrow([&] { grid.insert(std::make_unique<floah::LayoutElement>(), 0, 2); });
        expectThrow([&] { grid.insert(std::make_unique<floah::LayoutElement>(), 0, 2); });

        // Check they're in the right spot.
        compareEQ(grid.get(0, 0), &elem0);
        compareEQ(grid.get(1, 0), nullptr);
        compareEQ(grid.get(0, 1), nullptr);
        compareEQ(grid.get(1, 1), &elem1);
        expectThrow([&] { grid.get(2, 0); });
        expectThrow([&] { grid.get(0, 2); });

        // Check parent.
        compareEQ(elem0.getParent(), &grid);
        compareEQ(elem1.getParent(), &grid);

        // Overwrite.
        auto& elem2 = grid.insert(std::make_unique<floah::LayoutElement>(), 0, 0);
        compareEQ(grid.get(0, 0), &elem2);
        compareEQ(elem2.getParent(), &grid);

        // Remove.
        expectNoThrow([&] { grid.remove(0, 0); });
        expectNoThrow([&] { grid.remove(1, 0); });
        expectNoThrow([&] { grid.remove(0, 1); });
        expectNoThrow([&] { grid.remove(1, 1); });
        expectNoThrow([&] { grid.remove(1, 1); });
        expectThrow([&] { grid.remove(2, 0); });
        expectThrow([&] { grid.remove(0, 2); });

        // Extract.
        grid.insert(std::make_unique<floah::LayoutElement>(), 0, 0);
        const auto elem3 = grid.extract(0, 0);
        compareNE(elem3.get(), nullptr);
        compareEQ(elem3->getParent(), nullptr);
        compareEQ(grid.get(0, 0), nullptr);
        const auto elem4 = grid.extract(0, 0);
        compareEQ(elem4.get(), nullptr);
        expectThrow([&] { static_cast<void>(grid.extract(2, 0)); });
        expectThrow([&] { static_cast<void>(grid.extract(0, 2)); });
    }

    void GridAppendRow::operator()()
    {
        floah::Grid grid;

        // Append rows without active columns.
        compareEQ(grid.getRowCount(), 0);
        expectNoThrow([&] { grid.appendRow(); });
        compareEQ(grid.getRowCount(), 1);
        expectNoThrow([&] { grid.appendRow(); });
        compareEQ(grid.getRowCount(), 2);

        // Clear and add columns.
        grid.removeAllRowsAndColumns();
        grid.appendColumn();
        grid.appendColumn();
        grid.appendColumn();
        grid.appendColumn();

        // Append row.
        compareEQ(grid.getRowCount(), 0);
        expectNoThrow([&] { grid.appendRow(); });
        compareEQ(grid.get(0, 0), nullptr);
        compareEQ(grid.get(1, 0), nullptr);
        compareEQ(grid.get(2, 0), nullptr);
        compareEQ(grid.get(3, 0), nullptr);
        expectThrow([&] { grid.get(4, 0); });
        expectThrow([&] { grid.get(0, 1); });
        compareEQ(grid.getRowCount(), 1);

        // Insert some elements to check if they remain in the proper spot.
        auto& elem0 = grid.insert(std::make_unique<floah::LayoutElement>(), 0, 0);
        auto& elem1 = grid.insert(std::make_unique<floah::LayoutElement>(), 1, 0);
        auto& elem2 = grid.insert(std::make_unique<floah::LayoutElement>(), 2, 0);
        auto& elem3 = grid.insert(std::make_unique<floah::LayoutElement>(), 3, 0);

        // Append again and check elements are still at front.
        expectNoThrow([&] { grid.appendRow(); });
        compareEQ(grid.get(0, 0), &elem0);
        compareEQ(grid.get(1, 0), &elem1);
        compareEQ(grid.get(2, 0), &elem2);
        compareEQ(grid.get(3, 0), &elem3);
        compareEQ(grid.get(0, 1), nullptr);
        compareEQ(grid.get(1, 1), nullptr);
        compareEQ(grid.get(2, 1), nullptr);
        compareEQ(grid.get(3, 1), nullptr);
        expectThrow([&] { grid.get(4, 0); });
        expectThrow([&] { grid.get(0, 2); });
        compareEQ(grid.getRowCount(), 2);
    }

    void GridPrependRow::operator()()
    {
        floah::Grid grid;

        // Prepend rows without active columns.
        compareEQ(grid.getRowCount(), 0);
        expectNoThrow([&] { grid.prependRow(); });
        compareEQ(grid.getRowCount(), 1);
        expectNoThrow([&] { grid.prependRow(); });
        compareEQ(grid.getRowCount(), 2);

        // Clear and add columns.
        grid.removeAllRowsAndColumns();
        grid.appendColumn();
        grid.appendColumn();
        grid.appendColumn();
        grid.appendColumn();

        // Prepend row.
        compareEQ(grid.getRowCount(), 0);
        expectNoThrow([&] { grid.prependRow(); });
        compareEQ(grid.get(0, 0), nullptr);
        compareEQ(grid.get(1, 0), nullptr);
        compareEQ(grid.get(2, 0), nullptr);
        compareEQ(grid.get(3, 0), nullptr);
        expectThrow([&] { grid.get(4, 0); });
        expectThrow([&] { grid.get(0, 1); });
        compareEQ(grid.getRowCount(), 1);

        // Insert some elements to check if they remain in the proper spot.
        auto& elem0 = grid.insert(std::make_unique<floah::LayoutElement>(), 0, 0);
        auto& elem1 = grid.insert(std::make_unique<floah::LayoutElement>(), 1, 0);
        auto& elem2 = grid.insert(std::make_unique<floah::LayoutElement>(), 2, 0);
        auto& elem3 = grid.insert(std::make_unique<floah::LayoutElement>(), 3, 0);

        // Prepend again and check elements have shifted.
        expectNoThrow([&] { grid.prependRow(); });
        compareEQ(grid.get(0, 0), nullptr);
        compareEQ(grid.get(1, 0), nullptr);
        compareEQ(grid.get(2, 0), nullptr);
        compareEQ(grid.get(3, 0), nullptr);
        compareEQ(grid.get(0, 1), &elem0);
        compareEQ(grid.get(1, 1), &elem1);
        compareEQ(grid.get(2, 1), &elem2);
        compareEQ(grid.get(3, 1), &elem3);
        expectThrow([&] { grid.get(4, 0); });
        expectThrow([&] { grid.get(0, 2); });
        compareEQ(grid.getRowCount(), 2);
    }

    void GridInsertRow::operator()()
    {
        floah::Grid grid;

        // Insert rows without active columns.
        compareEQ(grid.getRowCount(), 0);
        expectNoThrow([&] { grid.insertRow(0); });
        compareEQ(grid.getRowCount(), 1);
        expectNoThrow([&] { grid.insertRow(0); });
        compareEQ(grid.getRowCount(), 2);
        expectNoThrow([&] { grid.insertRow(500); });
        compareEQ(grid.getRowCount(), 3);

        // Clear and add columns.
        grid.removeAllRowsAndColumns();
        grid.appendColumn();
        grid.appendColumn();
        grid.appendColumn();
        grid.appendColumn();

        // Insert row.
        compareEQ(grid.getRowCount(), 0);
        expectNoThrow([&] { grid.insertRow(0); });
        compareEQ(grid.get(0, 0), nullptr);
        compareEQ(grid.get(1, 0), nullptr);
        compareEQ(grid.get(2, 0), nullptr);
        compareEQ(grid.get(3, 0), nullptr);
        expectThrow([&] { grid.get(4, 0); });
        expectThrow([&] { grid.get(0, 1); });
        compareEQ(grid.getRowCount(), 1);

        // Insert some elements to check if they remain in the proper spot.
        auto& elem0 = grid.insert(std::make_unique<floah::LayoutElement>(), 0, 0);
        auto& elem1 = grid.insert(std::make_unique<floah::LayoutElement>(), 1, 0);
        auto& elem2 = grid.insert(std::make_unique<floah::LayoutElement>(), 2, 0);
        auto& elem3 = grid.insert(std::make_unique<floah::LayoutElement>(), 3, 0);

        // Insert again and check elements have shifted.
        expectNoThrow([&] { grid.insertRow(0); });
        compareEQ(grid.get(0, 0), nullptr);
        compareEQ(grid.get(1, 0), nullptr);
        compareEQ(grid.get(2, 0), nullptr);
        compareEQ(grid.get(3, 0), nullptr);
        compareEQ(grid.get(0, 1), &elem0);
        compareEQ(grid.get(1, 1), &elem1);
        compareEQ(grid.get(2, 1), &elem2);
        compareEQ(grid.get(3, 1), &elem3);
        expectThrow([&] { grid.get(4, 0); });
        expectThrow([&] { grid.get(0, 2); });
        compareEQ(grid.getRowCount(), 2);
    }

    void GridRemoveRow::operator()()
    {
        floah::Grid grid;

        // Remove rows without active columns.
        expectThrow([&] { grid.removeRow(0); });
        grid.appendRow();
        grid.appendRow();
        grid.appendRow();
        compareEQ(grid.getRowCount(), 3);
        expectNoThrow([&] { grid.removeRow(2); });
        compareEQ(grid.getRowCount(), 2);
        expectNoThrow([&] { grid.removeRow(1); });
        compareEQ(grid.getRowCount(), 1);
        expectNoThrow([&] { grid.removeRow(0); });
        compareEQ(grid.getRowCount(), 0);

        // Clear and add columns and rows.
        grid.removeAllRowsAndColumns();
        grid.appendRow();
        grid.appendRow();
        grid.appendRow();
        grid.appendColumn();
        grid.appendColumn();
        grid.appendColumn();
        grid.appendColumn();

        // Insert some elements to check if they remain in the proper spot.
        auto& elem0 = grid.insert(std::make_unique<floah::LayoutElement>(), 0, 0);
        auto& elem1 = grid.insert(std::make_unique<floah::LayoutElement>(), 1, 1);
        auto& elem2 = grid.insert(std::make_unique<floah::LayoutElement>(), 2, 1);
        grid.insert(std::make_unique<floah::LayoutElement>(), 3, 2);

        // Remove row and check remaining elements.
        expectNoThrow([&] { grid.removeRow(2); });
        compareEQ(grid.get(0, 0), &elem0);
        compareEQ(grid.get(1, 0), nullptr);
        compareEQ(grid.get(2, 0), nullptr);
        compareEQ(grid.get(3, 0), nullptr);
        compareEQ(grid.get(0, 1), nullptr);
        compareEQ(grid.get(1, 1), &elem1);
        compareEQ(grid.get(2, 1), &elem2);
        compareEQ(grid.get(3, 1), nullptr);
        compareEQ(grid.getRowCount(), 2);

        // Remove next row and check remaining elements.
        expectNoThrow([&] { grid.removeRow(0); });
        compareEQ(grid.get(0, 0), nullptr);
        compareEQ(grid.get(1, 0), &elem1);
        compareEQ(grid.get(2, 0), &elem2);
        compareEQ(grid.get(3, 0), nullptr);
        compareEQ(grid.getRowCount(), 1);

        // Remove final row.
        expectNoThrow([&] { grid.removeRow(0); });
        expectThrow([&] { grid.get(0, 0); });
        compareEQ(grid.getRowCount(), 0);
    }

    void GridExtractRow::operator()()
    {
        floah::Grid grid;

        // Add some elements.
        grid.appendRow();
        grid.appendRow();
        grid.appendRow();
        grid.appendColumn();
        grid.appendColumn();
        grid.appendColumn();
        grid.appendColumn();
        auto& elem0 = grid.insert(std::make_unique<floah::LayoutElement>(), 0, 0);
        auto& elem1 = grid.insert(std::make_unique<floah::LayoutElement>(), 1, 1);
        auto& elem2 = grid.insert(std::make_unique<floah::LayoutElement>(), 2, 1);
        auto& elem3 = grid.insert(std::make_unique<floah::LayoutElement>(), 3, 2);

        // And then extract rows one by one.
        auto elems = grid.extractRow(2);
        compareEQ(grid.getRowCount(), 2);
        compareEQ(elems.size(), 4).fatal("Extracted row has invalid size.");
        compareEQ(elems[0].get(), nullptr);
        compareEQ(elems[1].get(), nullptr);
        compareEQ(elems[2].get(), nullptr);
        compareEQ(elems[3].get(), &elem3);
        compareEQ(elems[3]->getParent(), nullptr);

        elems = grid.extractRow(0);
        compareEQ(grid.getRowCount(), 1);
        compareEQ(elems.size(), 4).fatal("Extracted row has invalid size.");
        compareEQ(elems[0].get(), &elem0);
        compareEQ(elems[1].get(), nullptr);
        compareEQ(elems[2].get(), nullptr);
        compareEQ(elems[3].get(), nullptr);
        compareEQ(elems[0]->getParent(), nullptr);

        elems = grid.extractRow(0);
        compareEQ(grid.getRowCount(), 0);
        compareEQ(elems.size(), 4).fatal("Extracted row has invalid size.");
        compareEQ(elems[0].get(), nullptr);
        compareEQ(elems[1].get(), &elem1);
        compareEQ(elems[2].get(), &elem2);
        compareEQ(elems[3].get(), nullptr);
        compareEQ(elems[1]->getParent(), nullptr);
        compareEQ(elems[2]->getParent(), nullptr);
    }

    void GridAppendColumn::operator()()
    {
        floah::Grid grid;

        // Append columns without active rows.
        compareEQ(grid.getColumnCount(), 0);
        expectNoThrow([&] { grid.appendColumn(); });
        compareEQ(grid.getColumnCount(), 1);
        expectNoThrow([&] { grid.appendColumn(); });
        compareEQ(grid.getColumnCount(), 2);

        // Clear and add rows.
        grid.removeAllRowsAndColumns();
        grid.appendRow();
        grid.appendRow();
        grid.appendRow();
        grid.appendRow();

        // Append column.
        compareEQ(grid.getColumnCount(), 0);
        expectNoThrow([&] { grid.appendColumn(); });
        compareEQ(grid.get(0, 0), nullptr);
        compareEQ(grid.get(0, 1), nullptr);
        compareEQ(grid.get(0, 2), nullptr);
        compareEQ(grid.get(0, 3), nullptr);
        expectThrow([&] { grid.get(1, 0); });
        expectThrow([&] { grid.get(0, 4); });
        compareEQ(grid.getColumnCount(), 1);

        // Insert some elements to check if they remain in the proper spot.
        auto& elem0 = grid.insert(std::make_unique<floah::LayoutElement>(), 0, 0);
        auto& elem1 = grid.insert(std::make_unique<floah::LayoutElement>(), 0, 1);
        auto& elem2 = grid.insert(std::make_unique<floah::LayoutElement>(), 0, 2);
        auto& elem3 = grid.insert(std::make_unique<floah::LayoutElement>(), 0, 3);

        // Append again and check elements are still in first column.
        expectNoThrow([&] { grid.appendColumn(); });
        compareEQ(grid.get(0, 0), &elem0);
        compareEQ(grid.get(0, 1), &elem1);
        compareEQ(grid.get(0, 2), &elem2);
        compareEQ(grid.get(0, 3), &elem3);
        compareEQ(grid.get(1, 0), nullptr);
        compareEQ(grid.get(1, 1), nullptr);
        compareEQ(grid.get(1, 2), nullptr);
        compareEQ(grid.get(1, 3), nullptr);
        expectThrow([&] { grid.get(2, 0); });
        expectThrow([&] { grid.get(0, 4); });
        compareEQ(grid.getColumnCount(), 2);
    }

    void GridPrependColumn::operator()()
    {
        floah::Grid grid;

        // Prepend columns without active rows.
        compareEQ(grid.getColumnCount(), 0);
        expectNoThrow([&] { grid.prependColumn(); });
        compareEQ(grid.getColumnCount(), 1);
        expectNoThrow([&] { grid.prependColumn(); });
        compareEQ(grid.getColumnCount(), 2);

        // Clear and add rows.
        grid.removeAllRowsAndColumns();
        grid.appendRow();
        grid.appendRow();
        grid.appendRow();
        grid.appendRow();

        // Prepend column.
        compareEQ(grid.getColumnCount(), 0);
        expectNoThrow([&] { grid.prependColumn(); });
        compareEQ(grid.get(0, 0), nullptr);
        compareEQ(grid.get(0, 1), nullptr);
        compareEQ(grid.get(0, 2), nullptr);
        compareEQ(grid.get(0, 3), nullptr);
        expectThrow([&] { grid.get(1, 0); });
        expectThrow([&] { grid.get(0, 4); });
        compareEQ(grid.getColumnCount(), 1);

        // Insert some elements to check if they remain in the proper spot.
        auto& elem0 = grid.insert(std::make_unique<floah::LayoutElement>(), 0, 0);
        auto& elem1 = grid.insert(std::make_unique<floah::LayoutElement>(), 0, 1);
        auto& elem2 = grid.insert(std::make_unique<floah::LayoutElement>(), 0, 2);
        auto& elem3 = grid.insert(std::make_unique<floah::LayoutElement>(), 0, 3);

        // Prepend again and check elements have shifted.
        expectNoThrow([&] { grid.prependColumn(); });
        compareEQ(grid.get(0, 0), nullptr);
        compareEQ(grid.get(0, 1), nullptr);
        compareEQ(grid.get(0, 2), nullptr);
        compareEQ(grid.get(0, 3), nullptr);
        compareEQ(grid.get(1, 0), &elem0);
        compareEQ(grid.get(1, 1), &elem1);
        compareEQ(grid.get(1, 2), &elem2);
        compareEQ(grid.get(1, 3), &elem3);
        expectThrow([&] { grid.get(2, 0); });
        expectThrow([&] { grid.get(0, 4); });
        compareEQ(grid.getColumnCount(), 2);
    }

    void GridInsertColumn::operator()()
    {
        floah::Grid grid;

        // Insert columns without active rows.
        compareEQ(grid.getColumnCount(), 0);
        expectNoThrow([&] { grid.insertColumn(0); });
        compareEQ(grid.getColumnCount(), 1);
        expectNoThrow([&] { grid.insertColumn(0); });
        compareEQ(grid.getColumnCount(), 2);
        expectNoThrow([&] { grid.insertColumn(500); });
        compareEQ(grid.getColumnCount(), 3);

        // Clear and add rows.
        grid.removeAllRowsAndColumns();
        grid.appendRow();
        grid.appendRow();
        grid.appendRow();
        grid.appendRow();

        // Insert column.
        compareEQ(grid.getColumnCount(), 0);
        expectNoThrow([&] { grid.insertColumn(0); });
        compareEQ(grid.get(0, 0), nullptr);
        compareEQ(grid.get(0, 1), nullptr);
        compareEQ(grid.get(0, 2), nullptr);
        compareEQ(grid.get(0, 3), nullptr);
        expectThrow([&] { grid.get(1, 0); });
        expectThrow([&] { grid.get(0, 4); });
        compareEQ(grid.getColumnCount(), 1);

        // Insert some elements to check if they remain in the proper spot.
        auto& elem0 = grid.insert(std::make_unique<floah::LayoutElement>(), 0, 0);
        auto& elem1 = grid.insert(std::make_unique<floah::LayoutElement>(), 0, 1);
        auto& elem2 = grid.insert(std::make_unique<floah::LayoutElement>(), 0, 2);
        auto& elem3 = grid.insert(std::make_unique<floah::LayoutElement>(), 0, 3);

        // Insert again and check elements have shifted.
        expectNoThrow([&] { grid.insertColumn(0); });
        compareEQ(grid.get(0, 0), nullptr);
        compareEQ(grid.get(0, 1), nullptr);
        compareEQ(grid.get(0, 2), nullptr);
        compareEQ(grid.get(0, 3), nullptr);
        compareEQ(grid.get(1, 0), &elem0);
        compareEQ(grid.get(1, 1), &elem1);
        compareEQ(grid.get(1, 2), &elem2);
        compareEQ(grid.get(1, 3), &elem3);
        expectThrow([&] { grid.get(2, 0); });
        expectThrow([&] { grid.get(0, 4); });
        compareEQ(grid.getColumnCount(), 2);
    }

    void GridRemoveColumn::operator()()
    {
        floah::Grid grid;

        // Remove columns without active rows.
        expectThrow([&] { grid.removeColumn(0); });
        grid.appendColumn();
        grid.appendColumn();
        grid.appendColumn();
        compareEQ(grid.getColumnCount(), 3);
        expectNoThrow([&] { grid.removeColumn(2); });
        compareEQ(grid.getColumnCount(), 2);
        expectNoThrow([&] { grid.removeColumn(1); });
        compareEQ(grid.getColumnCount(), 1);
        expectNoThrow([&] { grid.removeColumn(0); });
        compareEQ(grid.getColumnCount(), 0);

        // Clear and add columns and rows.
        grid.removeAllRowsAndColumns();
        grid.appendRow();
        grid.appendRow();
        grid.appendRow();
        grid.appendRow();
        grid.appendColumn();
        grid.appendColumn();
        grid.appendColumn();

        // Insert some elements to check if they remain in the proper spot.
        auto& elem0 = grid.insert(std::make_unique<floah::LayoutElement>(), 0, 0);
        auto& elem1 = grid.insert(std::make_unique<floah::LayoutElement>(), 1, 1);
        auto& elem2 = grid.insert(std::make_unique<floah::LayoutElement>(), 1, 2);
        grid.insert(std::make_unique<floah::LayoutElement>(), 2, 3);

        // Remove column and check remaining elements.
        expectNoThrow([&] { grid.removeColumn(2); });
        compareEQ(grid.get(0, 0), &elem0);
        compareEQ(grid.get(0, 1), nullptr);
        compareEQ(grid.get(0, 2), nullptr);
        compareEQ(grid.get(0, 3), nullptr);
        compareEQ(grid.get(1, 0), nullptr);
        compareEQ(grid.get(1, 1), &elem1);
        compareEQ(grid.get(1, 2), &elem2);
        compareEQ(grid.get(1, 3), nullptr);
        compareEQ(grid.getColumnCount(), 2);

        // Remove next column and check remaining elements.
        expectNoThrow([&] { grid.removeColumn(0); });
        compareEQ(grid.get(0, 0), nullptr);
        compareEQ(grid.get(0, 1), &elem1);
        compareEQ(grid.get(0, 2), &elem2);
        compareEQ(grid.get(0, 3), nullptr);
        compareEQ(grid.getColumnCount(), 1);

        // Remove final row.
        expectNoThrow([&] { grid.removeColumn(0); });
        expectThrow([&] { grid.get(0, 0); });
        compareEQ(grid.getColumnCount(), 0);
    }

    void GridExtractColumn::operator()()
    {
        floah::Grid grid;

        // Add some elements.
        grid.appendRow();
        grid.appendRow();
        grid.appendRow();
        grid.appendRow();
        grid.appendColumn();
        grid.appendColumn();
        grid.appendColumn();
        auto& elem0 = grid.insert(std::make_unique<floah::LayoutElement>(), 0, 0);
        auto& elem1 = grid.insert(std::make_unique<floah::LayoutElement>(), 1, 1);
        auto& elem2 = grid.insert(std::make_unique<floah::LayoutElement>(), 1, 2);
        auto& elem3 = grid.insert(std::make_unique<floah::LayoutElement>(), 2, 3);

        // And then extract columns one by one.
        auto elems = grid.extractColumn(2);
        compareEQ(grid.getColumnCount(), 2);
        compareEQ(elems.size(), 4).fatal("Extracted column has invalid size.");
        compareEQ(elems[0].get(), nullptr);
        compareEQ(elems[1].get(), nullptr);
        compareEQ(elems[2].get(), nullptr);
        compareEQ(elems[3].get(), &elem3);
        compareEQ(elems[3]->getParent(), nullptr);

        elems = grid.extractColumn(0);
        compareEQ(grid.getColumnCount(), 1);
        compareEQ(elems.size(), 4).fatal("Extracted column has invalid size.");
        compareEQ(elems[0].get(), &elem0);
        compareEQ(elems[1].get(), nullptr);
        compareEQ(elems[2].get(), nullptr);
        compareEQ(elems[3].get(), nullptr);
        compareEQ(elems[0]->getParent(), nullptr);

        elems = grid.extractColumn(0);
        compareEQ(grid.getColumnCount(), 0);
        compareEQ(elems.size(), 4).fatal("Extracted column has invalid size.");
        compareEQ(elems[0].get(), nullptr);
        compareEQ(elems[1].get(), &elem1);
        compareEQ(elems[2].get(), &elem2);
        compareEQ(elems[3].get(), nullptr);
        compareEQ(elems[1]->getParent(), nullptr);
        compareEQ(elems[2]->getParent(), nullptr);
    }
}  // namespace test

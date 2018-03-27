#include "TBTK/IndexedDataTree.h"
#include "TBTK/Model.h"

#include "gtest/gtest.h"

namespace TBTK{

TEST(IndexedDataTree, Constructor){
	//Not testable on its own.

}

//TODO
//...
TEST(IndexedDataTree, SerializeToJSON){
	//Serializable

	//Non/pseudo-serializable
}

TEST(IndexedDataTree, add){
	/**************************/
	/* Serializable elements. */
	/**************************/
	IndexedDataTree<Model> indexedDataTree0;

	//Adding elements with normal Index.
	indexedDataTree0.add(Model(), {1, 2, 3});
	indexedDataTree0.add(Model(), {1, 2, 4});

	//Adding element to already existing element (overwrite).
	indexedDataTree0.add(Model(), {1, 2, 3});

	//Adding elements with multiple indices.
	indexedDataTree0.add(Model(), {{1, 2, 5}, {1, 2, 3}});
	indexedDataTree0.add(Model(), {{1, 2, 5}, {1, 2, 4}});

	//Adding element to already existing element (overwrite) for element
	//with multiple indices.
	indexedDataTree0.add(Model(), {{1, 2, 5}, {1, 2, 3}});

	//Fail to add element with multiple indices if an element with a single
	//Index with the same structure already have been added.
	EXPECT_EXIT(
		{
			Streams::setStdMuteErr();
			indexedDataTree0.add(Model(), {{1, 2, 3}, {1, 2, 3}});
		},
		::testing::ExitedWithCode(1),
		""
	);

	//Fail to add element with if the Index clashes with multiple Index
	//structure that already have been added.
	EXPECT_EXIT(
		{
			Streams::setStdMuteErr();
			indexedDataTree0.add(Model(), {{1, 2, 5}});
		},
		::testing::ExitedWithCode(1),
		""
	);

	//Fail to add element if the Index clashes with an Index structure that
	//already has been added (longer version).
	EXPECT_EXIT(
		{
			Streams::setStdMuteErr();
			indexedDataTree0.add(Model(), {{1, 2, 3, 4}});
		},
		::testing::ExitedWithCode(1),
		""
	);

	//Fail to add element if the Index clashes with an Index structure that
	//already has been added (shorter version).
	EXPECT_EXIT(
		{
			Streams::setStdMuteErr();
			indexedDataTree0.add(Model(), {{1, 2}});
		},
		::testing::ExitedWithCode(1),
		""
	);

	//Fail to add element if the Index clashes with multiple Index
	//structure that already has been added.
	EXPECT_EXIT(
		{
			Streams::setStdMuteErr();
			indexedDataTree0.add(Model(), {{1, 2, 5, 6}});
		},
		::testing::ExitedWithCode(1),
		""
	);

	//Fail to add Index with negative Index (Except for the negative
	//IDX_SEPARATOR already tested above).
	EXPECT_EXIT(
		{
			Streams::setStdMuteErr();
			indexedDataTree0.add(Model(), {1, -1, 3});
		},
		::testing::ExitedWithCode(1),
		""
	);

	/*************************************/
	/* Non/pseudo-serializable elements. */
	/*************************************/
	IndexedDataTree<double> indexedDataTree1;

	//Adding elements with normal Index.
	indexedDataTree1.add(0, {1, 2, 3});
	indexedDataTree1.add(0, {1, 2, 4});

	//Adding element to already existing element (overwrite).
	indexedDataTree1.add(0, {1, 2, 3});

	//Adding elements with multiple indices.
	indexedDataTree1.add(0, {{1, 2, 5}, {1, 2, 3}});
	indexedDataTree1.add(0, {{1, 2, 5}, {1, 2, 4}});

	//Adding element to already existing element (overwrite) for element
	//with multiple indices.
	indexedDataTree1.add(0, {{1, 2, 5}, {1, 2, 3}});

	//Fail to add element with multiple indices if an element with a single
	//Index with the same structure already have been added.
	EXPECT_EXIT(
		{
			Streams::setStdMuteErr();
			indexedDataTree1.add(0, {{1, 2, 3}, {1, 2, 3}});
		},
		::testing::ExitedWithCode(1),
		""
	);

	//Fail to add element with if the Index clashes with multiple Index
	//structure that already have been added.
	EXPECT_EXIT(
		{
			Streams::setStdMuteErr();
			indexedDataTree1.add(0, {{1, 2, 5}});
		},
		::testing::ExitedWithCode(1),
		""
	);

	//Fail to add element if the Index clashes with an Index structure that
	//already has been added (longer version).
	EXPECT_EXIT(
		{
			Streams::setStdMuteErr();
			indexedDataTree1.add(0, {{1, 2, 3, 4}});
		},
		::testing::ExitedWithCode(1),
		""
	);

	//Fail to add element if the Index clashes with an Index structure that
	//already has been added (shorter version).
	EXPECT_EXIT(
		{
			Streams::setStdMuteErr();
			indexedDataTree1.add(0, {{1, 2}});
		},
		::testing::ExitedWithCode(1),
		""
	);

	//Fail to add element if the Index clashes with multiple Index
	//structure that already has been added.
	EXPECT_EXIT(
		{
			Streams::setStdMuteErr();
			indexedDataTree1.add(0, {{1, 2, 5, 6}});
		},
		::testing::ExitedWithCode(1),
		""
	);

	//Fail to add Index with negative Index (Except for the negative
	//IDX_SEPARATOR already tested above).
	EXPECT_EXIT(
		{
			Streams::setStdMuteErr();
			indexedDataTree1.add(0, {1, -1, 3});
		},
		::testing::ExitedWithCode(1),
		""
	);
}

TEST(IndexedDataTree, get){
	/**************************/
	/* Serializable elements. */
	/**************************/

	Streams::setStdMuteOut();

	//Setup Models that will work as test elements.
	Model model0;
	model0 << HoppingAmplitude(1, {0}, {0});
	model0.construct();

	Model model1;
	model1 << HoppingAmplitude(1, {0}, {0});
	model1 << HoppingAmplitude(1, {1}, {1});
	model1.construct();

	Model model2;
	model2 << HoppingAmplitude(1, {0}, {0});
	model2 << HoppingAmplitude(1, {1}, {1});
	model2 << HoppingAmplitude(1, {2}, {2});
	model2.construct();

	Model model3;
	model3 << HoppingAmplitude(1, {0}, {0});
	model3 << HoppingAmplitude(1, {1}, {1});
	model3 << HoppingAmplitude(1, {2}, {2});
	model3 << HoppingAmplitude(1, {3}, {3});
	model3.construct();

	//Actual tests
	IndexedDataTree<Model> indexedDataTree0;
	indexedDataTree0.add(model0, {1, 2, 3});
	indexedDataTree0.add(model1, {1, 2, 4});
	indexedDataTree0.add(model2, {1, 2, 3});
	indexedDataTree0.add(model3, {{1, 2, 5}, {1, 2, 3}});

	Model model;
	//Access existing elements.
	EXPECT_TRUE(indexedDataTree0.get(model, {1, 2, 3}));
	EXPECT_EQ(model.getBasisSize(), 3);
	EXPECT_TRUE(indexedDataTree0.get(model, {1, 2, 4}));
	EXPECT_EQ(model.getBasisSize(), 2);
	EXPECT_TRUE(indexedDataTree0.get(model, {{1, 2, 5}, {1, 2, 3}}));
	EXPECT_EQ(model.getBasisSize(), 4);

	//Access non-existing element
	EXPECT_FALSE(indexedDataTree0.get(model, {1, 2, 2}));
	EXPECT_FALSE(indexedDataTree0.get(model, {1, 1, 3}));

	//Fail to access Index with negative Index (Except for the negative
	//IDX_SEPARATOR already tested above).
	EXPECT_EXIT(
		{
			Streams::setStdMuteErr();
			indexedDataTree0.get(model, {1, -1, 3});
		},
		::testing::ExitedWithCode(1),
		""
	);

	/*************************************/
	/* Non/pseudo-serializable elements. */
	/*************************************/
	IndexedDataTree<int> indexedDataTree1;
	indexedDataTree1.add(1, {1, 2, 3});
	indexedDataTree1.add(2, {1, 2, 4});
	indexedDataTree1.add(3, {1, 2, 3});
	indexedDataTree1.add(4, {{1, 2, 5}, {1, 2, 3}});

	int myInt;
	//Access existing elements.
	EXPECT_TRUE(indexedDataTree1.get(myInt, {1, 2, 3}));
	EXPECT_EQ(myInt, 3);
	EXPECT_TRUE(indexedDataTree1.get(myInt, {1, 2, 4}));
	EXPECT_EQ(myInt, 2);
	EXPECT_TRUE(indexedDataTree1.get(myInt, {{1, 2, 5}, {1, 2, 3}}));
	EXPECT_EQ(myInt, 4);

	//Access non-existing element
	EXPECT_FALSE(indexedDataTree1.get(myInt, {1, 2, 2}));
	EXPECT_FALSE(indexedDataTree1.get(myInt, {1, 1, 3}));

	//Fail to access Index with negative Index (Except for the negative
	//IDX_SEPARATOR already tested above).
	EXPECT_EXIT(
		{
			Streams::setStdMuteErr();
			indexedDataTree1.get(myInt, {1, -1, 3});
		},
		::testing::ExitedWithCode(1),
		""
	);
}

TEST(IndexedDataTree, clear){
	/**************************/
	/* Serializable elements. */
	/**************************/
	IndexedDataTree<Model> indexedDataTree0;
	indexedDataTree0.add(Model(), {1, 2, 3});
	indexedDataTree0.add(Model(), {1, 2, 4});
	indexedDataTree0.add(Model(), {{1, 2, 5}, {1, 2, 3}});
	indexedDataTree0.clear();

	Model model;
	EXPECT_FALSE(indexedDataTree0.get(model, {1, 2, 3}));
	EXPECT_FALSE(indexedDataTree0.get(model, {1, 2, 4}));
	EXPECT_FALSE(indexedDataTree0.get(model, {{1, 2, 5}, {1, 2, 3}}));

	/*************************************/
	/* Non/pseudo-serializable elements. */
	/*************************************/
	IndexedDataTree<int> indexedDataTree1;
	indexedDataTree1.add(1, {1, 2, 3});
	indexedDataTree1.add(1, {1, 2, 4});
	indexedDataTree1.add(1, {{1, 2, 5}, {1, 2, 3}});
	indexedDataTree1.clear();

	int myInt;
	EXPECT_FALSE(indexedDataTree1.get(myInt, {1, 2, 3}));
	EXPECT_FALSE(indexedDataTree1.get(myInt, {1, 2, 4}));
	EXPECT_FALSE(indexedDataTree1.get(myInt, {{1, 2, 5}, {1, 2, 3}}));
}

TEST(IndexedDataTree, getSizeInBytes){
	/**************************/
	/* Serializable elements. */
	/**************************/
	IndexedDataTree<Model> indexedDataTree0;
	EXPECT_TRUE(indexedDataTree0.getSizeInBytes() > 0);

	/*************************************/
	/* Non/pseudo-serializable elements. */
	/*************************************/
	IndexedDataTree<int> indexedDataTree1;
	EXPECT_TRUE(indexedDataTree1.getSizeInBytes() > 0);
}

};
default:
	g++ -o witchertracker src/main.cpp src/Inventory.cpp src/Monster.cpp src/Potion.cpp src/Utils.cpp

grade:
	python3 test/grader.py ./witchertracker test-cases

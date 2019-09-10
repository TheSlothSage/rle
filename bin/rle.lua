rle.newTilemap("test", 100, 100, 1)
rle.loadComponent("helloworld")
rle.loadSystem("helloworld")

rle.newEntity("hello", "test", 0, 0, 0):addComponent("helloworld")


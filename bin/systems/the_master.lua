systems.the_master = {}

systems.the_master.functions = { "Init", "Update" }

function systems.the_master.Init ()
        print("first?")
        rle.this():getComponent("texture"):setStr("path", "assets/special/people/the_master.png") 
end

function systems.the_master.Update () 

end

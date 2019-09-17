systems.texture = {}

systems.texture.functions = { "Init", "Update" }

function systems.texture.Init ()
	rle.sdl_pushTextureToTable(rle.this():getComponent("texture"):getStr("path"))
end

function systems.texture.Update () 

end

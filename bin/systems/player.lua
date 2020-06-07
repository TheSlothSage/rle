systems.player = {}

systems.player.functions = { "Init", "Update" }

function systems.player.Init ()	
end

function systems.player.Update () 
    
    key = rle.this():getComponent("keyboard"):getStr("key_down")

    if key == "A" then
        if rle.this().x > 0 then
            rle.this().x = rle.this().x - 1
        end
    elseif key == "D" then
        if rle.this().x < rle.this():getTileMap().x - 1 then
            rle.this().x = rle.this().x + 1
        end
    elseif key == "S" then
        if rle.this().y > 0 then
            rle.this().y = rle.this().y - 1
        end
    elseif key == "W" then
        if rle.this().y < rle.this():getTileMap().y - 1 then
            rle.this().y = rle.this().y + 1
        end 
    end
end

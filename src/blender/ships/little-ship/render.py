import bpy
import math

bpy.data.objects["Wasser-Helper"].hide_render = True

bpy.data.scenes["Scene"].render.resolution_percentage = 100
for i in range(0, 360, 45):
    bpy.data.objects["Setup"].rotation_euler.z = i / 180 * math.pi
    bpy.context.scene.render.filepath = "render/angle" + str(i) + ".png"
    bpy.ops.render.render(write_still = True)
include_directories(
	"."
)

add_library(reMafia STATIC
	"Memory.cpp"
	"I3D_math.cpp"
	"C_chunk.cpp"
	"C_mission.cpp"
	"C_game.cpp"
	"C_tshift.cpp"
	"C_roads.cpp"
	"C_web_path.cpp"
	"C_program.cpp"
	"g_collision.cpp"
	"C_Vehicle.cpp"
	"C_using_object.cpp"
	"C_traffic_element.cpp"
	"G_IndicatorsClass.cpp"
	"Actors/C_actor.cpp"
	"Actors/C_clock.cpp"
	"Actors/C_door.cpp"
	"Actors/C_human.cpp"
	"Actors/C_player.cpp"
    "Actors/C_car.cpp"
	"Actors/C_PhThing.cpp"
	"Actors/C_public_physics.cpp"
	"Actors/C_box_new.cpp"
	"Actors/C_crash_obj.cpp"
    "Actors/C_DropOut.cpp"
	"Actors/C_detector.cpp"
	"Actors/C_traffic_generator.cpp"
	
)

set_property(TARGET reMafia PROPERTY CXX_STANDARD 98)
set_property(TARGET reMafia PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_options(reMafia PUBLIC "$<$<C_COMPILER_ID:MSVC>:-SAFESEH:NO>")
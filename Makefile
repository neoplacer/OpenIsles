SRC_DIRECTORY := src
DATA_DIRECTORY := data

# Für das fertige Tileset für Tiled: Anzahl Kacheln pro Zeile
TILESET_WIDTH := 4

CREATE_TARGET_DIRECTORY = mkdir -p $(@D)

MONTAGE := montage -background transparent

.PHONY: all clean build-gui clean-gui render-cart render-coat-of-arms render-blender clean-blender

all: build-gui render-blender

clean: clean-gui clean-blender


########################################################################################################################
# Gelände-Kacheln                                                                                                      #
########################################################################################################################

$(DATA_DIRECTORY)/img/tileset.png: $(SRC_DIRECTORY)/blender/tiles/tiles.blend
	mkdir -p $(DATA_DIRECTORY)/img/tiles
	cd $(SRC_DIRECTORY)/blender/tiles; blender -b $(notdir $<) -P render.py
	php $(SRC_DIRECTORY)/blender/tiles/generate-tileset.php

	cp $(SRC_DIRECTORY)/blender/tiles/render/tileset.png $(DATA_DIRECTORY)/img/tileset.png

########################################################################################################################
# GUI                                                                                                                  #
########################################################################################################################
	
clean-gui:
	rm -f $(DATA_DIRECTORY)/img/gui/panel.png
	rm -f $(DATA_DIRECTORY)/img/gui/statusbar.png
	rm -f $(DATA_DIRECTORY)/img/gui/add-building/add-building-grid.png
	
build-gui: $(DATA_DIRECTORY)/img/gui/panel.png $(DATA_DIRECTORY)/img/gui/statusbar.png $(DATA_DIRECTORY)/img/gui/add-building/add-building-grid.png


$(DATA_DIRECTORY)/img/gui/panel.png:
	$(CREATE_TARGET_DIRECTORY)
	convert -size 236x748 canvas:"#907f67" -mattecolor "#6f5038" -frame 10x10+5+5 $@
	
$(DATA_DIRECTORY)/img/gui/statusbar.png:
	$(CREATE_TARGET_DIRECTORY)
	convert -size 758x24 canvas:"#907f67" -mattecolor "#6f5038" -frame 5x5+2+2 $@

$(DATA_DIRECTORY)/img/gui/add-building/add-building-grid.png: $(SRC_DIRECTORY)/blender/ui/add-building-grid/add-building-grid.blend
	$(CREATE_TARGET_DIRECTORY)
	cd $(SRC_DIRECTORY)/blender/ui/add-building-grid; blender -b $(notdir $<) -P render.py
	mv $(SRC_DIRECTORY)/blender/ui/add-building-grid/add-building-grid.png $@
	
########################################################################################################################
# Gebäude                                                                                                              #
########################################################################################################################

BUILDINGS := butchers cathedral cattle-farm chapel foresters hunters-hut marketplace office1 \
             pioneers-house1 pioneers-house2 pioneers-house3 pioneers-house4 pioneers-house5 \
             sheep-farm stonemason tavern toolsmiths weaving-mill1

define RENDER_BUILDING
$(DATA_DIRECTORY)/img/objects/$(1).png: $(SRC_DIRECTORY)/blender/buildings/$(1)/$(1).blend
	$$(CREATE_TARGET_DIRECTORY)
	cd $(SRC_DIRECTORY)/blender/buildings/$(1); blender -b $$(notdir $$<) -P ../render-building.py

	# geometry muss angegeben werden, sonst greift der Default von 120x120
	$(MONTAGE) \
	    $(SRC_DIRECTORY)/blender/buildings/$(1)/render/angle0.png \
	    $(SRC_DIRECTORY)/blender/buildings/$(1)/render/angle90.png \
	    $(SRC_DIRECTORY)/blender/buildings/$(1)/render/angle180.png \
	    $(SRC_DIRECTORY)/blender/buildings/$(1)/render/angle270.png \
	    -geometry +0+0 -tile 4x1 $$@
endef

$(foreach BUILDING,$(BUILDINGS),$(eval $(call RENDER_BUILDING,$(BUILDING))))

########################################################################################################################
# Straßen                                                                                                              #
########################################################################################################################

$(DATA_DIRECTORY)/img/objects/streets.png: $(SRC_DIRECTORY)/blender/streets/streets.blend
	mkdir -p $(DATA_DIRECTORY)/img/objects
	cd $(SRC_DIRECTORY)/blender/streets; blender -b $(notdir $<) -P render-streets.py

	# geometry muss angegeben werden, sonst greift der Default von 120x120
	$(MONTAGE) \
	    $(SRC_DIRECTORY)/blender/streets/render/street-straight0.png \
	    $(SRC_DIRECTORY)/blender/streets/render/street-straight90.png \
	    $(SRC_DIRECTORY)/blender/streets/render/street-curve0.png \
	    $(SRC_DIRECTORY)/blender/streets/render/street-curve90.png \
	    $(SRC_DIRECTORY)/blender/streets/render/street-curve180.png \
	    $(SRC_DIRECTORY)/blender/streets/render/street-curve270.png \
	    $(SRC_DIRECTORY)/blender/streets/render/street-tee0.png \
	    $(SRC_DIRECTORY)/blender/streets/render/street-tee90.png \
	    $(SRC_DIRECTORY)/blender/streets/render/street-tee180.png \
	    $(SRC_DIRECTORY)/blender/streets/render/street-tee270.png \
	    $(SRC_DIRECTORY)/blender/streets/render/street-cross.png \
	    -geometry +0+0 -tile x1 $@

########################################################################################################################
# Gütersymbole                                                                                                         #
########################################################################################################################

GOODS := bricks cattle cloth food tools wood wool

define RENDER_GOODS_ICONS
$(DATA_DIRECTORY)/img/goods/marketplace-icon/$(1).png $(DATA_DIRECTORY)/img/goods/icon/$(1).png: \
	$(SRC_DIRECTORY)/blender/goods/$(1)/$(1).blend \
	$(SRC_DIRECTORY)/blender/goods/render.py \
	$(SRC_DIRECTORY)/xcf/goods/marketplace-icon-background.xcf
	
	mkdir -p $(DATA_DIRECTORY)/img/goods/marketplace-icon
	mkdir -p $(DATA_DIRECTORY)/img/goods/icon
	
	cd $$(dir $$<); blender -b $$(notdir $$<) -P ../render.py
	
	mv -f $$(dir $$<)icon.png $(DATA_DIRECTORY)/img/goods/icon/$(1).png
	convert $(SRC_DIRECTORY)/xcf/goods/marketplace-icon-background.xcf -resize 42x42 -flatten $$(dir $$<)marketplace-icon.png -gravity center -composite $(DATA_DIRECTORY)/img/goods/marketplace-icon/$(1).png
	rm -f $$(dir $$<)marketplace-icon.png
endef

$(foreach GOOD,$(GOODS),$(eval $(call RENDER_GOODS_ICONS,$(GOOD))))

########################################################################################################################
# Animationen                                                                                                          #
########################################################################################################################

ANIMATIONS := carrier

define RENDER_ANIMATION
$(DATA_DIRECTORY)/img/objects/$(1).png: $(SRC_DIRECTORY)/blender/animations/$(1)/$(1).blend
	$$(CREATE_TARGET_DIRECTORY)
	cd $(SRC_DIRECTORY)/blender/animations/$(1); blender -b $$(notdir $$<) -P ../render-animation.py

	# geometry muss angegeben werden, sonst greift der Default von 120x120
	$(MONTAGE) \
        $(SRC_DIRECTORY)/blender/animations/$(1)/render/angle0/* \
        $(SRC_DIRECTORY)/blender/animations/$(1)/render/angle45/* \
        $(SRC_DIRECTORY)/blender/animations/$(1)/render/angle90/* \
        $(SRC_DIRECTORY)/blender/animations/$(1)/render/angle135/* \
        $(SRC_DIRECTORY)/blender/animations/$(1)/render/angle180/* \
        $(SRC_DIRECTORY)/blender/animations/$(1)/render/angle225/* \
        $(SRC_DIRECTORY)/blender/animations/$(1)/render/angle270/* \
        $(SRC_DIRECTORY)/blender/animations/$(1)/render/angle315/* \
        -geometry +0+0 -tile x8 $$@
endef

$(foreach ANIMATION,$(ANIMATIONS),$(eval $(call RENDER_ANIMATION,$(ANIMATION))))

########################################################################################################################
# Marktkarren-Animation                                                                                                #
########################################################################################################################

render-cart: $(SRC_DIRECTORY)/blender/animations/cart/cart.blend
	mkdir -p $(DATA_DIRECTORY)/img/objects
	cd $(SRC_DIRECTORY)/blender/animations/cart; blender -b $(notdir $<) -P render-cart.py

	# geometry muss angegeben werden, sonst greift der Default von 120x120
	$(MONTAGE) \
	    $(SRC_DIRECTORY)/blender/animations/cart/render/without_cargo/angle0/* \
	    $(SRC_DIRECTORY)/blender/animations/cart/render/without_cargo/angle45/* \
	    $(SRC_DIRECTORY)/blender/animations/cart/render/without_cargo/angle90/* \
	    $(SRC_DIRECTORY)/blender/animations/cart/render/without_cargo/angle135/* \
	    $(SRC_DIRECTORY)/blender/animations/cart/render/without_cargo/angle180/* \
	    $(SRC_DIRECTORY)/blender/animations/cart/render/without_cargo/angle225/* \
	    $(SRC_DIRECTORY)/blender/animations/cart/render/without_cargo/angle270/* \
	    $(SRC_DIRECTORY)/blender/animations/cart/render/without_cargo/angle315/* \
	    -geometry +0+0 -tile x8 $(DATA_DIRECTORY)/img/objects/cart-without-cargo.png
	$(MONTAGE) \
	    $(SRC_DIRECTORY)/blender/animations/cart/render/with_cargo/angle0/* \
	    $(SRC_DIRECTORY)/blender/animations/cart/render/with_cargo/angle45/* \
	    $(SRC_DIRECTORY)/blender/animations/cart/render/with_cargo/angle90/* \
	    $(SRC_DIRECTORY)/blender/animations/cart/render/with_cargo/angle135/* \
	    $(SRC_DIRECTORY)/blender/animations/cart/render/with_cargo/angle180/* \
	    $(SRC_DIRECTORY)/blender/animations/cart/render/with_cargo/angle225/* \
	    $(SRC_DIRECTORY)/blender/animations/cart/render/with_cargo/angle270/* \
	    $(SRC_DIRECTORY)/blender/animations/cart/render/with_cargo/angle315/* \
	    -geometry +0+0 -tile x8 $(DATA_DIRECTORY)/img/objects/cart-with-cargo.png

########################################################################################################################
# Banner                                                                                                               #
########################################################################################################################

render-coat-of-arms: $(SRC_DIRECTORY)/blender/ui/coat-of-arms/coat-of-arms.blend $(DATA_DIRECTORY)/img/population-man.png
	mkdir -p $(DATA_DIRECTORY)/img/gui/coat-of-arms
	cd $(SRC_DIRECTORY)/blender/ui/coat-of-arms; blender -b coat-of-arms.blend -P render.py

	cp -rf $(SRC_DIRECTORY)/blender/ui/coat-of-arms/render/* $(DATA_DIRECTORY)/img/gui/coat-of-arms
	rm -rf $(SRC_DIRECTORY)/blender/ui/coat-of-arms/render

	mkdir $(DATA_DIRECTORY)/img/gui/coat-of-arms/population
	for color in blue red white yellow; \
		do convert -background transparent $(DATA_DIRECTORY)/img/gui/coat-of-arms/small/$$color.png \
		           -gravity north-west -extent 30x30 \
		           $(DATA_DIRECTORY)/img/population-man.png -geometry +11+0 -composite \
		           $(DATA_DIRECTORY)/img/gui/coat-of-arms/population/$$color.png; \
	done

########################################################################################################################
# Sonstige Blender-Sachen                                                                                              #
########################################################################################################################

$(DATA_DIRECTORY)/img/coin.png: $(SRC_DIRECTORY)/blender/ui/coin/coin.blend
	mkdir -p $(DATA_DIRECTORY)/img/objects
	blender -b $< -o //coin\#.png -f 1
	mv $(SRC_DIRECTORY)/blender/ui/coin/coin1.png $@
	
$(DATA_DIRECTORY)/img/population-man.png: $(SRC_DIRECTORY)/blender/ui/population-man/population-man.blend
	mkdir -p $(DATA_DIRECTORY)/img/objects
	blender -b $< -o //population-man\#.png -f 1
	mv $(SRC_DIRECTORY)/blender/ui/population-man/population-man1.png $@

########################################################################################################################
# PHONYs um alle Blender-Sachen zu rendern und zu cleanen                                                              #
########################################################################################################################

render-blender: \
	$(foreach BUILDING,$(BUILDINGS), \
	    $(DATA_DIRECTORY)/img/objects/$(BUILDING).png \
	) \
	$(foreach GOOD,$(GOODS), \
	    $(DATA_DIRECTORY)/img/goods/marketplace-icon/$(GOOD).png \
	    $(DATA_DIRECTORY)/img/goods/icon/$(GOOD).png \
	) \
	$(foreach ANIMATION,$(ANIMATIONS), \
		$(DATA_DIRECTORY)/img/objects/$(ANIMATION).png \
	) \
	$(DATA_DIRECTORY)/img/tileset.png \
	$(DATA_DIRECTORY)/img/objects/streets.png \
	render-cart \
	render-coat-of-arms \
	$(DATA_DIRECTORY)/img/coin.png \
	$(DATA_DIRECTORY)/img/population-man.png
	
clean-blender:
	rm -f $(foreach BUILDING,$(BUILDINGS), $(DATA_DIRECTORY)/img/objects/$(BUILDING).png)
	rm -rf $(foreach BUILDING,$(BUILDINGS), $(SRC_DIRECTORY)/blender/buildings/$(BUILDING)/render)
	rm -rf $(DATA_DIRECTORY)/img/goods
	rm -f $(foreach ANIMATION,$(ANIMATIONS), $(DATA_DIRECTORY)/img/objects/$(ANIMATION).png)
	rm -rf $(foreach ANIMATION,$(ANIMATIONS), $(SRC_DIRECTORY)/blender/animations/$(ANIMATION)/render)
	rm -rf $(SRC_DIRECTORY)/blender/animations/cart/render
	rm -rf $(DATA_DIRECTORY)/img/tileset.png
	rm -rf $(SRC_DIRECTORY)/blender/tiles/render
	rm -rf $(SRC_DIRECTORY)/blender/streets/render
	rm -rf $(DATA_DIRECTORY)/img/objects/streets.png
	rm -rf $(DATA_DIRECTORY)/img/objects/cart-without-cargo.png
	rm -rf $(DATA_DIRECTORY)/img/objects/cart-with-cargo.png
	rm -rf $(DATA_DIRECTORY)/img/gui/coat-of-arms
	rm -rf $(DATA_DIRECTORY)/img/coin.png
	rm -rf $(DATA_DIRECTORY)/img/population-man.png

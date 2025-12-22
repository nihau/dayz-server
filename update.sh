#!/bin/bash
set -euo pipefail

### PATHS
STEAMCMD="/home/dayz/servers/steamcmd/steamcmd.sh"
SERVER_DIR="/home/dayz/servers/dayz-server"
WORKSHOP_DIR="$SERVER_DIR/steamapps/workshop/content/221100"
KEYS_DIR="$SERVER_DIR/keys"

### STEAM LOGIN
STEAM_LOGIN="nihau93"

### MODS (WorkshopID => FolderName)
declare -A MODS=(
  [1559212036]="@CF"
  [1564026768]="@Community-Online-Tools"
  [2545327648]="@DabsFramework"
  [2291785308]="@Core"
  [3628768530]="@Gunther3"
  #[2381686295]="@Minimap"
  #[2925837634]="@MiniMapZoom"
)

MOD_LIST=""

for id in "${!MODS[@]}"; do
  MOD_LIST+="${MODS[$id]};"
done

# remove trailing ;
MOD_LIST="${MOD_LIST%;}"

ENV_FILE="/home/dayz/servers/dayz-server/mods.env"

cat > "$ENV_FILE" <<EOF
MODS=${MOD_LIST}
EOF

echo "Generated $ENV_FILE:"
cat "$ENV_FILE"

echo "================================================"
echo " Updating DayZ Server & Workshop Mods"
echo "================================================"

### UPDATE SERVER + DOWNLOAD MODS
#+app_update 223350 validate \ возможно валидейт перетирает нахуй всё
"$STEAMCMD" \
  +force_install_dir "$SERVER_DIR" \
  +login "$STEAM_LOGIN" \
  +app_update 223350 \
$(for MOD_ID in "${!MODS[@]}"; do
    echo "  +workshop_download_item 221100 $MOD_ID validate"
  done) \
  +quit

echo
echo "================================================"
echo " Installing Mods"
echo "================================================"

### INSTALL MODS
for MOD_ID in "${!MODS[@]}"; do
  MOD_FOLDER="${MODS[$MOD_ID]}"
  SRC="$WORKSHOP_DIR/$MOD_ID"
  DST="$SERVER_DIR/$MOD_FOLDER"

  echo " Installing $MOD_FOLDER ($MOD_ID)"

  if [ ! -d "$SRC" ]; then
    echo " ERROR: Workshop download missing for $MOD_ID"
    exit 1
  fi

  rm -rf "$DST"
  mkdir -p "$DST"
  cp -a "$SRC/." "$DST/"
done

echo
echo "================================================"
echo " Installing Keys"
echo "================================================"

mkdir -p "$KEYS_DIR"

### COPY ALL BIKEYS
#for MOD_ID in "${!MODS[@]}"; do
#  SRC_KEYS="$WORKSHOP_DIR/$MOD_ID/keys"
#  if [ -d "$SRC_KEYS" ]; then
#    echo " Copying keys from $MOD_ID"
#    cp -f "$SRC_KEYS/"*.bikey "$KEYS_DIR/" 2>/dev/null || true
#  fi
#done
find "$WORKSHOP_DIR" -type f -iname "*.bikey" -exec cp -f {} "$KEYS_DIR/" \; || true

echo
echo "================================================"
echo " Fixing Permissions"
echo "================================================"

chmod -R 755 "$SERVER_DIR"/@*
chmod -R 755 "$KEYS_DIR"
find "$KEYS_DIR" -name "*.bikey" -exec chmod 644 {} \;

echo
echo "================================================"
echo " Update Complete"
echo "================================================"

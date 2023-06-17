#!/bin/bash

# Make sure current directory is where the script is
script_location="$(cd "$(dirname "$0")" && pwd)"
cd $script_location

# Update Files that Depend on AZAD Repo
function copy_azad_file() {
    file_name="$1"
    source_path="azad/src/js/$file_name"
    destination_path="../../src/azad/$file_name"
    # Check if source file exists
    if [ ! -f "$source_path" ]; then
        echo "Source file does not exist."
        return 1
    fi
    # Copy the file to the destination, overwriting if it already exists
    echo $source_path
    echo $destination_path
    cp -f $source_path $destination_path
}

AZAD_URL="https://github.com/philipmulcahy/azad.git"
mkdir tmp
cd tmp
git clone $AZAD_URL # TODO: Maybe try to install git first?
copy_azad_file "inject.ts"
copy_azad_file "order.ts"
copy_azad_file "csv.ts"
copy_azad_file "table.ts"
copy_azad_file "notice.ts"
copy_azad_file "request_scheduler.ts"
copy_azad_file "signin.ts"
copy_azad_file "settings.ts"
copy_azad_file "statistics.ts"
copy_azad_file "url.ts"
copy_azad_file "util.ts"
cd ..
rm -rf tmp

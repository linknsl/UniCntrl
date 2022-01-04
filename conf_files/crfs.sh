#!/bin/bash

dname=`dirname "$1"`
bname=`basename "$1"`
start_path="`cd \"$dname\" 2>/dev/null && pwd || echo \"$dname\"`/$bname"

echo "File system root: $start_path"
# creating init.sh
current_date=$(date +%m%d%H%M%Y)
current_date1=$(date)
current_date1=$(date +%s)

# creating README.txt
cat << EOF > ./README.txt

File system building information
--------------------------------

build_date = ${current_date1}

sdk_prefix = ${mv7sft}

lib        = \${sdk_prefix}/arm${eb}-mv7sft-linux-gnueabi/libc/lib

cmd_prefix = \${sdk_prefix}/bin/arm${eb}-mv7sft-linux-gnueabi-

gdbserver  = ${gdbserver_file}

EOF

cat << EOF > ./dateinit.sh
#!/bin/sh
  date $current_date
EOF

chmod 777 dateinit.sh

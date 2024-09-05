require 'mkmf'

$CFLAGS = "-I/chdb.h"
$LDFLAGS = " -lchdb"

create_makefile('chdb_ext')
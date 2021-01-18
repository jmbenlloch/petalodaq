import tables as tb
import os
import pytest
import json
import numpy as np

from subprocess import check_output
from pytest     import mark


@pytest.fixture(scope = 'session')
def RD_DIR():
    return os.environ['RD_DIR']

@pytest.fixture(scope = 'session')
def tmpdir(tmpdir_factory):
    return tmpdir_factory.mktemp('rawdata')

@mark.slow
def test_split_outputfile(tmpdir, RD_DIR):
    #run without splitting
    fileout    = str(tmpdir) + '/' + '6323.h5'
    filein   = RD_DIR + '/testing/samples/' + 'run_6323.rd'
    data = {"file_in" : filein,
            "file_out": fileout}

    config_file = fileout + '.json'
    with open(config_file, 'w') as outfile:
        json.dump(data, outfile)

    cmd = '{}/decode {}'.format(RD_DIR, config_file)
    output = check_output(cmd, shell=True, executable='/bin/bash')

    #open output files
    h5out       = tb.open_file(fileout)

    # Check run info
    runinfo      = h5out     .root.Run.runInfo[:][0]

    # Close files
    h5out.close()


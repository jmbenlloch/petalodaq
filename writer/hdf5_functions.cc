#include "writer/hdf5_functions.h"

hid_t createRunType(){
	hsize_t memtype = H5Tcreate (H5T_COMPOUND, sizeof (runinfo_t));
	H5Tinsert (memtype, "run_number", HOFFSET (runinfo_t, run_number), H5T_NATIVE_INT);
	return memtype;
}

hid_t createTable(hid_t group, std::string& table_name, hsize_t memtype){
	//Create 1D dataspace (evt number). First dimension is unlimited (initially 0)
	const hsize_t ndims = 1;
	hsize_t dims[ndims] = {0};
	hsize_t max_dims[ndims] = {H5S_UNLIMITED};
	hsize_t file_space = H5Screate_simple(ndims, dims, max_dims);

	// Create a dataset creation property list
	// The layout of the dataset have to be chunked when using unlimited dimensions
	hid_t plist = H5Pcreate(H5P_DATASET_CREATE);
	H5Pset_layout(plist, H5D_CHUNKED);
	hsize_t chunk_dims[ndims] = {32768};
	H5Pset_chunk(plist, ndims, chunk_dims);

	// Create dataset
	hid_t dataset = H5Dcreate(group, table_name.c_str(), memtype, file_space,
			H5P_DEFAULT, plist, H5P_DEFAULT);
	return dataset;
}

hid_t createGroup(hid_t file, std::string& groupName){
	//Create group
	hid_t wfgroup;
	wfgroup = H5Gcreate2(file, groupName.c_str(), H5P_DEFAULT, H5P_DEFAULT,
			H5P_DEFAULT);
	return wfgroup;
}

void writeRun(runinfo_t * runData, hid_t dataset, hid_t memtype, hsize_t evt_number){
	hid_t memspace, file_space;
	hsize_t dims[1] = {1};
	memspace = H5Screate_simple(1, dims, NULL);

	//Extend PMT dataset
	dims[0] = evt_number+1;
	H5Dset_extent(dataset, dims);

	file_space = H5Dget_space(dataset);
	hsize_t start[1] = {evt_number};
	hsize_t count[1] = {1};
	H5Sselect_hyperslab(file_space, H5S_SELECT_SET, start, NULL, count, NULL);
	H5Dwrite(dataset, memtype, memspace, file_space, H5P_DEFAULT, runData);
	H5Sclose(file_space);
	H5Sclose(memspace);
}

hid_t createTofPetType(){
	//Create compound datatype for the table
	hsize_t memtype = H5Tcreate (H5T_COMPOUND, sizeof (petalo_t));
	H5Tinsert (memtype, "evt_number"  , HOFFSET (petalo_t, evt_number ), H5T_NATIVE_UINT);
	H5Tinsert (memtype, "card_id"     , HOFFSET (petalo_t, card_id    ), H5T_NATIVE_UINT16);
	H5Tinsert (memtype, "tofpet_id"   , HOFFSET (petalo_t, tofpet_id  ), H5T_NATIVE_UINT8);
	H5Tinsert (memtype, "wordtype_id" , HOFFSET (petalo_t, wordtype_id), H5T_NATIVE_UINT8);
	H5Tinsert (memtype, "channel_id"  , HOFFSET (petalo_t, channel_id ), H5T_NATIVE_UINT8);
	H5Tinsert (memtype, "sensor_id"   , HOFFSET (petalo_t, sensor_id  ), H5T_NATIVE_INT16);
	H5Tinsert (memtype, "tac_id"      , HOFFSET (petalo_t, tac_id     ), H5T_NATIVE_UINT8);
	H5Tinsert (memtype, "tcoarse"     , HOFFSET (petalo_t, tcoarse    ), H5T_NATIVE_UINT16);
	H5Tinsert (memtype, "ecoarse"     , HOFFSET (petalo_t, ecoarse    ), H5T_NATIVE_UINT16);
	H5Tinsert (memtype, "tfine"       , HOFFSET (petalo_t, tfine      ), H5T_NATIVE_UINT16);
	H5Tinsert (memtype, "efine"       , HOFFSET (petalo_t, efine      ), H5T_NATIVE_UINT16);
	return memtype;
}

hid_t createEvtCounterType(){
	//Create compound datatype for the table
	hsize_t memtype = H5Tcreate (H5T_COMPOUND, sizeof (evt_counter_t));
	H5Tinsert (memtype, "evt_number"  , HOFFSET (evt_counter_t, evt_number ), H5T_NATIVE_UINT);
	H5Tinsert (memtype, "card_id"     , HOFFSET (evt_counter_t, card_id    ), H5T_NATIVE_UINT16);
	H5Tinsert (memtype, "tofpet_id"   , HOFFSET (evt_counter_t, tofpet_id  ), H5T_NATIVE_UINT8);
	H5Tinsert (memtype, "wordtype_id" , HOFFSET (evt_counter_t, wordtype_id), H5T_NATIVE_UINT8);
	H5Tinsert (memtype, "reserved"    , HOFFSET (evt_counter_t, reserved   ), H5T_NATIVE_UINT);
	H5Tinsert (memtype, "channel_id"  , HOFFSET (evt_counter_t, channel_id ), H5T_NATIVE_UINT8);
	H5Tinsert (memtype, "sensor_id"   , HOFFSET (evt_counter_t, sensor_id  ), H5T_NATIVE_INT16);
	H5Tinsert (memtype, "count"       , HOFFSET (evt_counter_t, count      ), H5T_NATIVE_UINT);
	return memtype;
}

void writeTofPet(petalo_t * data, hid_t dataset, hid_t memtype, hsize_t evt_number){
	hid_t memspace, file_space;
	hsize_t dims[1] = {1};
	memspace = H5Screate_simple(1, dims, NULL);

	//Extend PMT dataset
	dims[0] = evt_number+1;
	H5Dset_extent(dataset, dims);

	file_space = H5Dget_space(dataset);
	hsize_t start[1] = {evt_number};
	hsize_t count[1] = {1};
	H5Sselect_hyperslab(file_space, H5S_SELECT_SET, start, NULL, count, NULL);
	H5Dwrite(dataset, memtype, memspace, file_space, H5P_DEFAULT, data);
	H5Sclose(file_space);
	H5Sclose(memspace);
}

void writeEvtCount(evt_counter_t * data, hid_t dataset, hid_t memtype, hsize_t evt_number){
	hid_t memspace, file_space;
	hsize_t dims[1] = {1};
	memspace = H5Screate_simple(1, dims, NULL);

	//Extend PMT dataset
	dims[0] = evt_number+1;
	H5Dset_extent(dataset, dims);

	file_space = H5Dget_space(dataset);
	hsize_t start[1] = {evt_number};
	hsize_t count[1] = {1};
	H5Sselect_hyperslab(file_space, H5S_SELECT_SET, start, NULL, count, NULL);
	H5Dwrite(dataset, memtype, memspace, file_space, H5P_DEFAULT, data);
	H5Sclose(file_space);
	H5Sclose(memspace);
}

#include <target/scs_slot.h>
#include <target/ifd.h>

__near__ scs_sid_t ifd_sids[NR_IFD_SLOTS];

scs_sid_t ifd_slot_sid(ifd_sid_t sid)
{
	BUG_ON(sid >= NR_IFD_SLOTS);
	return ifd_sids[sid];
}

ifd_sid_t ifd_slot_id(scs_sid_t sid)
{
	ifd_sid_t id;
	for (id = 0; id < NR_IFD_SLOTS; id++) {
		if (sid == ifd_sids[id])
			return id;
	}
	BUG();
	return INVALID_IFD_SID;
}

scs_err_t ifd_slot_error(scs_err_t err)
{
	switch (err) {
	case SCS_ERR_SUCCESS:
	case SCS_ERR_PROGRESS:
	case SCS_ERR_OVERRUN:
	case SCS_ERR_TIMEOUT:
	case SCS_ERR_NOTPRESENT:
		return err;
	default:
		return SCS_ERR_HW_ERROR;
	}
}

uint8_t ifd_slot_status(uint8_t state)
{
	switch (state) {
	case IFD_SLOT_STATE_ATR_READY:
		return SCS_SLOT_STATUS_ACTIVE;
	case IFD_SLOT_STATE_NOTPRESENT:
		return SCS_SLOT_STATUS_NOTPRESENT;
	case IFD_SLOT_STATE_PPS_READY:
	case IFD_SLOT_STATE_PRESENT:
	case IFD_SLOT_STATE_SELECTED:
	case IFD_SLOT_STATE_ACTIVATED:
	case IFD_SLOT_STATE_HWERROR:
	default:
		return SCS_SLOT_STATUS_INACTIVE;
	}
}

static void ifd_slot_select(void)
{
	ifd_sid_select(ifd_slot_id(scs_sid));
}

static scs_err_t ifd_slot_activate(void)
{
	scs_err_t err;
	err = ifd_slot_activation(true);
	return ifd_slot_error(err);
}

static scs_err_t ifd_slot_deactivate(void)
{
	scs_err_t err;
	err = ifd_power_off();
	return ifd_slot_error(err);
}

static scs_err_t ifd_slot_xchg_block(scs_size_t nc, scs_size_t ne)
{
	scs_err_t err;
	err = ifd_xchg_block(nc, ne);
	return ifd_slot_error(err);
}

static scs_size_t ifd_slot_xchg_avail(void)
{
	return ifd_xchg_avail();
}

static scs_err_t ifd_slot_xchg_write(scs_off_t index, uint8_t byte)
{
	scs_err_t err;
	err = ifd_write_byte(index, byte);
	return ifd_slot_error(err);
}

static uint8_t ifd_slot_xchg_read(scs_off_t index)
{
	return ifd_read_byte(index);
}

scs_slot_driver_t ifd_slot = {
	ifd_slot_select,
	ifd_slot_activate,
	ifd_slot_deactivate,
	ifd_slot_xchg_block,
	ifd_slot_xchg_avail,
	ifd_slot_xchg_write,
	ifd_slot_xchg_read,
};

void ifd_slot_completion(scs_err_t err)
{
	scs_slot_select(ifd_slot_sid(ifd_slid));
	scs_complete_slot(ifd_slot_error(err));
}

void ifd_slot_synchronization(uint8_t status)
{
	scs_slot_select(ifd_slot_sid(ifd_slid));
	scs_set_slot_status(ifd_slot_status(status));
}

void ifd_slot_init()
{
	ifd_sid_t sid;
	for (sid = 0; sid < NR_IFD_SLOTS; sid++) {
		ifd_sids[sid] = scs_register_slot(&ifd_slot);
	}
}

package com.ford.syncV4.protocol;

import android.test.AndroidTestCase;

import com.ford.syncV4.protocol.enums.FrameDataControlFrameType;
import com.ford.syncV4.protocol.enums.FrameType;
import com.ford.syncV4.protocol.enums.SessionType;

/**
 * Created by Andrew Batutin on 8/20/13.
 */
public class ProtocolFrameHeaderFactoryTest extends AndroidTestCase {

    public ProtocolFrameHeaderFactoryTest() {
    }

    public void testMobileNavStartSessionFrameCreation() throws Exception {
        ProtocolFrameHeader header = ProtocolFrameHeaderFactory.createMobileNavStartSession(48, (byte) 48, (byte) 2);
        assertEquals(header.getSessionType(), SessionType.Mobile_Nav);
        assertEquals(header.getFrameType(), FrameType.Control);
        assertEquals(header.getFrameData(), FrameDataControlFrameType.StartSession.getValue());
        assertEquals(header.getVersion(), (byte) 2);
        assertEquals(header.isCompressed(), false);
        assertEquals(header.getDataSize(), 0x00);
        assertTrue(header.getMessageID() == 48);
        assertTrue(header.getSessionID() == (byte) 48);
    }

    public void testSessionStartACKFrameCreation() throws Exception {
        ProtocolFrameHeader header = ProtocolFrameHeaderFactory.createStartSessionACK(SessionType.RPC, (byte) 48, 48, (byte) 2);
        byte[] data =  header.assembleHeaderBytes();
        assertNotNull("data should not be null", data);
    }

    public void testSessionStartNACKFrameCreation() throws Exception {
        ProtocolFrameHeader header = ProtocolFrameHeaderFactory.createStartSessionNACK(SessionType.Mobile_Nav, (byte) 48, 48, (byte) 2);
        byte[] data =  header.assembleHeaderBytes();
        assertNotNull("data should not be null", data);
    }

    public void testMobileNavStartSessionACKFrameCreation() throws Exception {
        ProtocolFrameHeader header = ProtocolFrameHeaderFactory.createStartSessionACK(SessionType.Mobile_Nav, (byte) 48, 48, (byte) 2);
        assertEquals(header.getSessionType(), SessionType.Mobile_Nav);
        assertEquals(header.getFrameType(), FrameType.Control);
        assertEquals(header.getFrameData(), FrameDataControlFrameType.StartSessionACK.getValue());
        assertEquals(header.getVersion(), (byte) 2);
        assertEquals(header.isCompressed(), false);
        assertEquals(header.getDataSize(), 0x00);
        assertTrue(header.getMessageID() == 48);
        assertTrue(header.getSessionID() == (byte) 48);
    }

    public void testMobileNavEndSessionFrameCreation() throws Exception {
        ProtocolFrameHeader header = ProtocolFrameHeaderFactory.createEndSession(SessionType.Mobile_Nav, (byte) 48, 48, (byte) 2, 0);
        assertEquals(header.getSessionType(), SessionType.Mobile_Nav);
        assertEquals(header.getFrameType(), FrameType.Control);
        assertEquals(header.getFrameData(), FrameDataControlFrameType.EndSession.getValue());
        assertEquals(header.getVersion(), (byte) 2);
        assertEquals(header.isCompressed(), false);
        assertEquals(header.getDataSize(), 0x00);
        assertTrue(header.getMessageID() == 48);
        assertTrue(header.getSessionID() == (byte) 48);
    }

    public void testMobileNavDataSessionFrameCreation() throws Exception {
        ProtocolFrameHeader header = ProtocolFrameHeaderFactory.createSingleSendData(SessionType.Mobile_Nav, (byte) 48, 10, 48, (byte) 2);
        assertEquals(header.getSessionType(), SessionType.Mobile_Nav);
        assertEquals(header.getFrameType(), FrameType.Single);
        assertEquals(header.getFrameData(), 0x00);
        assertEquals(header.getVersion(), (byte) 2);
        assertEquals(header.isCompressed(), false);
        assertEquals(header.getDataSize(), 10);
        assertTrue(header.getMessageID() == 48);
        assertTrue(header.getSessionID() == (byte) 48);
    }

    public void testMobileNavAckFrameCreation() throws Exception {
        ProtocolFrameHeader header = ProtocolFrameHeaderFactory.createMobileNaviAck(SessionType.Mobile_Nav, (byte) 48, 48, (byte) 2);
        assertEquals(header.getSessionType(), SessionType.Mobile_Nav);
        assertEquals(header.getFrameType(), FrameType.Control);
        assertEquals(header.getFrameData(), FrameDataControlFrameType.MobileNaviACK.getValue());
        assertEquals(header.getVersion(), (byte) 2);
        assertEquals(header.isCompressed(), false);
        assertEquals(header.getDataSize(), 0);
        assertTrue(header.getMessageID() == 48);
        assertTrue(header.getSessionID() == (byte) 48);
    }
}
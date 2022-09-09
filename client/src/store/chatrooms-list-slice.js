import { createSlice } from "@reduxjs/toolkit";

const initialState = {
  roomsList: [],    
};

export const chatRoomsListSlice = createSlice({
  name: 'chatRooms',
  initialState,
  reducers: {
    addChatRoom: (state, action) => {
      state.roomsList.push(action.payload);
    },
    setChatRooms: (state, action) => {
      state.roomsList = action.payload;
    }
  }
});

export const { addChatRoom, setChatRooms } = chatRoomsListSlice.actions;
export default chatRoomsListSlice.reducer;
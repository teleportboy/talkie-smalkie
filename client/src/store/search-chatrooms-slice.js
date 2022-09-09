import { createSlice } from "@reduxjs/toolkit";

const initialState = {
  chatrooms: []
};

export const searchChatRoomsSlice = createSlice({
  name: 'searchChatRooms',
  initialState,
  reducers: {
    updateSearchedChatRooms: (state, action) => {
      state.chatrooms = action.payload;
    }
  }
});

export const { updateSearchedChatRooms } = searchChatRoomsSlice.actions;
export default searchChatRoomsSlice.reducer;
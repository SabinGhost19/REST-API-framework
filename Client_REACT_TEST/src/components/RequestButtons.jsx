import axios from "axios";
import { useState } from "react";
const RequestButtons=()=>{

    const apiUrl = 'http://localhost:8080';
    const [response,setResponse]=useState("");
    // return (
    //     <div className=' font-sans w-96 space-y-4 mt-10 flex flex-col text-white '>
    //     <button className='   shadow rounded py-2.5  bg-indigo-600 hover:bg-indigo-500 block w-full rounded-md '>GET</button>
    //     <button className=' shadow rounded py-2.5  bg-indigo-600 hover:bg-indigo-500 block w-full rounded-md'>POST</button>
    //     <button className=' shadow rounded py-2.5 bg-indigo-600 hover:bg-indigo-500 block w-full rounded-md'>DELETE</button>
    //     </div>
    // );
    const handleGet=async()=>{
        try {
            const response=await axios.get(`${apiUrl}/home`);
            console.log('GET Response:', response.data);
            setResponse(JSON.stringify(response.data,null,2));

        } catch (error) {
            console.error('Error with GET request:', error);
        }
    }
    return (
        <div className="flex items-center justify-center min-h-screen">
          <div className="font-sans w-full max-w-md space-y-6 mt-10 flex flex-col items-center text-white">
            <button onClick={handleGet}className="shadow-lg rounded-lg py-4 px-8 text-lg bg-indigo-600 hover:bg-indigo-500 w-full">GET</button>
            <pre className="bg-gray-800 p-4 rounded-lg w-full mt-4 overflow-auto text-left">
            {response}
            </pre>
            <button className="shadow-lg rounded-lg py-4 px-8 text-lg bg-indigo-600 hover:bg-indigo-500 w-full">POST</button>
            <button className="shadow-lg rounded-lg py-4 px-8 text-lg bg-indigo-600 hover:bg-indigo-500 w-full">PUT</button>
            <button className="shadow-lg rounded-lg py-4 px-8 text-lg bg-indigo-600 hover:bg-indigo-500 w-full">PATCH</button>
            <button className="shadow-lg rounded-lg py-4 px-8 text-lg bg-indigo-600 hover:bg-indigo-500 w-full">DELETE</button>
          </div>
        </div>
      );

}
export default RequestButtons;
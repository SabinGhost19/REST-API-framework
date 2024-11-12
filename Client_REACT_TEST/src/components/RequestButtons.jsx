import axios from 'axios';
import { useEffect, useState } from 'react';

const RequestButtons = () => {
  const apiUrl = 'http://localhost:8081';
  const [response, setResponse] = useState('');
  const [htmlData, setHtmlData] = useState('');

  const handleGet = async () => {
    try {
      const response = await axios.get(`${apiUrl}/home`);
      console.log('GET Response:', response.data);
      setResponse(JSON.stringify(response.data, null, 2));
    } catch (error) {
      console.error('Error with GET request:', error);
    }
  };

  const handleGET_HTML = async () => {
    try {
      const response = await axios.get(`${apiUrl}/htmlFile`, {
        headers: {
          'Content-Type': 'text/html',
        },
        responseType: 'text',
      });

      console.log('GET HTML Response:', response.data);
      setHtmlData(response.data);
    } catch (error) {
      console.error('Error with GET request:', error);
    }
  };

  //randare continut new html primit de la server
  //si crearea unui nou tab
  //useEffect pentru ca atunci cand se modifica continutul
  //htmlData sa se activat si astfel deschisa si randata
  // noua pagina cu continutul primit
  useEffect(() => {
    if (htmlData) {
      const newWindow = window.open();
      if (newWindow) {
        newWindow.document.open();
        newWindow.document.write(htmlData);
        newWindow.document.close();
      }
    }
  }, [htmlData]);

  return (
    <div className="flex items-center justify-center min-h-screen">
      <div className="font-sans w-full max-w-md space-y-6 mt-10 flex flex-col items-center text-white">
        <button
          onClick={handleGet}
          className="shadow-lg rounded-lg py-4 px-8 text-lg bg-indigo-600 hover:bg-indigo-500 w-full"
        >
          GET
        </button>
        <pre className="bg-gray-800 p-4 rounded-lg w-full mt-4 overflow-auto text-left">
          {response}
        </pre>
        <button
          onClick={handleGET_HTML}
          className="shadow-lg rounded-lg py-4 px-8 text-lg bg-indigo-600 hover:bg-indigo-500 w-full"
        >
          GET HTML
        </button>
        <button className="shadow-lg rounded-lg py-4 px-8 text-lg bg-indigo-600 hover:bg-indigo-500 w-full">
          PUT
        </button>
        <button className="shadow-lg rounded-lg py-4 px-8 text-lg bg-indigo-600 hover:bg-indigo-500 w-full">
          PATCH
        </button>
        <button className="shadow-lg rounded-lg py-4 px-8 text-lg bg-indigo-600 hover:bg-indigo-500 w-full">
          DELETE
        </button>
      </div>
    </div>
  );
};

export default RequestButtons;

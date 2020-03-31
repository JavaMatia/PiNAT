package com.pinat.pinatclient.ui.tabs;

import android.content.Context;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.TextView;

import androidx.cardview.widget.CardView;
import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.pinat.pinatclient.Adapters.DevicesTabAdapter;
import com.pinat.pinatclient.R;

import org.w3c.dom.Text;

import java.util.Arrays;
import java.util.List;

/**
 * A placeholder fragment containing a simple view.
 */
public class FragmentTab1 extends Fragment {

    private static Context context;

    public FragmentTab1(Context c) {
        super();
        context = c;
    }

    @Override
    public View onCreateView(
            @NonNull LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState)
    {
        View rootView = inflater.inflate(R.layout.fragment_tab1, container, false);

        int len = getArguments().getString("devices").length();
        String[] macs = getArguments().getString("devices").
                substring(1, len-1).split(",");

        List<String> macList = Arrays.asList(macs);

        updateTab(macList, rootView);
        return rootView;
    }

    public void updateTab(List<String> macs, View rootView)
    {
        RecyclerView recyclerView = rootView.findViewById(R.id.devicesRecyclerView);
        DevicesTabAdapter devicesTabAdapter;
        recyclerView.hasFixedSize();

        recyclerView.setLayoutManager(new LinearLayoutManager(context));
        devicesTabAdapter = new DevicesTabAdapter(macs, context);
        recyclerView.setAdapter(devicesTabAdapter);
    }

}